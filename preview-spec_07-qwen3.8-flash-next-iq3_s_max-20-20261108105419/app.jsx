// App root: header, tabs, clock, URL query sync, bookings state, toasts.

const App = () => {
  const [now, setNow] = useState(() => new Date());
  const [bookings, setBookings] = useState(() => loadBookings());
  const [view, setView] = useState("timeline");
  const [filter, setFilter] = useState(() => parseTimelineQuery(location.search));
  const [toasts, setToasts] = useState([]);
  const [modal, setModal] = useState(null);
  const [cancelTarget, setCancelTarget] = useState(null);
  const [savingId, setSavingId] = useState(null);

  useEffect(() => {
    const t = setInterval(() => setNow(new Date()), 30000);
    return () => clearInterval(t);
  }, []);

  useEffect(() => {
    history.replaceState(null, "", buildTimelineQuery(filter));
  }, [filter]);

  const pushToast = useCallback((t) => {
    const id = genId();
    setToasts((prev) => [...prev, { id, ...t }]);
    setTimeout(() => setToasts((prev) => prev.filter((x) => x.id !== id)), 3000);
  }, []);

  const dismissToast = useCallback((id) => setToasts((prev) => prev.filter((x) => x.id !== id)), []);

  const applyBookings = useCallback((next) => {
    setBookings(next);
    saveBookings(next);
  }, []);

  const bookingToastBody = (v) =>
    `${getRoomById(v.roomId).name} · ${fmtDateShort(v.date)} ${fmtRange(v.start, v.end)}`;

  const commitBooking = (values) => {
    const isEdit = bookings.some((b) => b.id === values.id);
    applyBookings(isEdit ? bookings.map((b) => (b.id === values.id ? values : b)) : [...bookings, values]);
    pushToast({
      kind: "success",
      title: isEdit ? "已更新預約" : "預約成功",
      body: bookingToastBody(values),
    });
  };

  const openCreate = (slot) => {
    const room = getRoomById(slot.roomId);
    setModal({
      mode: "create",
      init: { ...slot, title: "", attendees: Math.min(6, room.capacity), notes: "" },
    });
  };

  const openEdit = (b) => setModal({ mode: "edit", init: b });

  const patchBookingTime = async (id, start, end) => {
    const orig = bookings.find((b) => b.id === id);
    if (!orig) return;
    const updated = { ...orig, start, end };
    applyBookings(bookings.map((b) => (b.id === id ? updated : b)));
    setSavingId(id);
    try {
      await simulateApi({ title: orig.title });
      pushToast({
        kind: "success",
        title: "已更新時間",
        body: `${getRoomById(orig.roomId).name} · ${fmtDateShort(orig.date)} ${fmtRange(start, end)}`,
      });
    } catch (e) {
      setBookings((prev) => prev.map((b) => (b.id === id ? orig : b)));
      saveBookings(bookings);
      pushToast({ kind: "error", title: "更新失敗", body: "請稍後再試" });
    } finally {
      setSavingId(null);
    }
  };

  const confirmCancel = (b) => {
    applyBookings(bookings.filter((x) => x.id !== b.id));
    setCancelTarget(null);
    pushToast({ kind: "success", title: "已取消預約", body: `${b.title} ${fmtDateShort(b.date)} ${fmtRange(b.start, b.end)}` });
  };

  const resetDemo = () => {
    applyBookings(buildSeedBookings());
    pushToast({ kind: "success", title: "示範資料已重置" });
  };

  return (
    <div className="app-shell">
      <header className="top-bar">
        <div className="brand">
          <span className="brand-logo">{Icons.calendar({ size: 17 })}</span>
          會議室預約
        </div>
        <nav className="nav-tabs" aria-label="主要導覽">
          <button type="button" className={`nav-tab${view === "timeline" ? " active" : ""}`} aria-current={view === "timeline"} onClick={() => setView("timeline")}>
            時間表
          </button>
          <button type="button" className={`nav-tab${view === "mine" ? " active" : ""}`} aria-current={view === "mine"} onClick={() => setView("mine")}>
            我的預約
          </button>
        </nav>
        <div className="top-user">
          <span className="avatar" aria-hidden="true">王</span>
          {CURRENT_USER}
        </div>
      </header>

      <main className="page-body">
        {view === "timeline" ? (
          <div className="panel">
            <TimelineView
              bookings={bookings}
              filter={filter}
              setFilter={setFilter}
              now={now}
              onCreateSlot={openCreate}
              onOpenEdit={openEdit}
              onPatchBookingTime={patchBookingTime}
              pushToast={pushToast}
              savingId={savingId}
            />
          </div>
        ) : (
          <MyBookingsView
            bookings={bookings}
            now={now}
            onEdit={openEdit}
            onCancel={setCancelTarget}
            onGoTimeline={() => setView("timeline")}
          />
        )}
      </main>

      <footer className="footer-bar">
        <span>資料保存在瀏覽器 localStorage · 使用者：{CURRENT_USER}</span>
        <button type="button" className="btn" onClick={resetDemo}>重置示範資料</button>
      </footer>

      {modal ? (
        <BookingModal
          mode={modal.mode}
          initial={modal.init}
          bookings={bookings}
          onClose={() => setModal(null)}
          onCommit={commitBooking}
          pushToast={pushToast}
        />
      ) : null}
      {cancelTarget ? (
        <CancelConfirm
          booking={cancelTarget}
          onClose={() => setCancelTarget(null)}
          onConfirm={confirmCancel}
        />
      ) : null}
      <ToastHost toasts={toasts} onDismiss={dismissToast} />
    </div>
  );
};

ReactDOM.createRoot(document.getElementById("root")).render(<App />);
