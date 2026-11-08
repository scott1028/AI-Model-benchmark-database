// Timeline: desktop grid + mobile vertical list, slot select, booking cards,
// drag-resize engine (R-19~R-22). Overlays use position:fixed so the scroll
// container never clips them.

const pctOfMin = (m) => ((m - DAY_START_MIN) / (DAY_END_MIN - DAY_START_MIN)) * 100;

const useIsMobile = () => {
  const [mobile, setMobile] = useState(
    () => typeof window !== "undefined" && window.matchMedia("(max-width: 767.98px)").matches
  );
  useEffect(() => {
    const mq = window.matchMedia("(max-width: 767.98px)");
    const onChange = (e) => setMobile(e.matches);
    mq.addEventListener("change", onChange);
    return () => mq.removeEventListener("change", onChange);
  }, []);
  return mobile;
};

const slotStartMin = (i) => DAY_START_MIN + i * SLOT_MIN;

const BookingCard = ({ booking, isMine, locked, saving, flagged, dragging, clampSide, axis, onOpen, onHandleDown, suppressUntilRef }) => {
  const [hover, setHover] = useState(false);
  const [stickyTip, setStickyTip] = useState(false);
  const [tipPos, setTipPos] = useState(null);
  const cardRef = useRef(null);

  const showTip = (hover || stickyTip) && !dragging && tipPos;

  const enterTip = () => {
    setHover(true);
    const r = cardRef.current.getBoundingClientRect();
    setTipPos({ left: Math.min(r.left, window.innerWidth - 270), top: r.top - 8 });
  };

  const s = timeToMin(booking.start);
  const e = timeToMin(booking.end);
  const style =
    axis === "x"
      ? { left: `${pctOfMin(s)}%`, width: `${pctOfMin(e) - pctOfMin(s)}%` }
      : { top: `${pctOfMin(s)}%`, height: `calc(${pctOfMin(e) - pctOfMin(s)}% - 4px)` };

  const cls = [
    "bk-card",
    isMine ? "mine" : "",
    isMine && !locked ? "clickable" : "",
    (hover || dragging) && isMine && !locked ? "own-hover" : "",
    dragging ? "dragging" : "",
    flagged ? "flagged" : "",
    saving ? "saving" : "",
  ]
    .filter(Boolean)
    .join(" ");

  const label =
    `${booking.title} ${booking.date} ${fmtRange(booking.start, booking.end)} ` +
    `預約人 ${booking.bookedBy} 參與人數 ${booking.attendees} ` +
    (isMine ? "我的預約" : "已被預約");

  return (
    <div
      ref={cardRef}
      className={cls}
      style={style}
      role="button"
      tabIndex={0}
      aria-label={label}
      onMouseEnter={enterTip}
      onMouseLeave={() => {
        setHover(false);
        setStickyTip(false);
      }}
      onClick={() => {
        if (suppressUntilRef && suppressUntilRef.current > Date.now()) return;
        if (isMine && !locked) onOpen(booking);
        else {
          enterTip();
          setStickyTip(true);
        }
      }}
      onKeyDown={(ev) => {
        if (ev.key === "Enter" || ev.key === " ") {
          ev.preventDefault();
          if (suppressUntilRef && suppressUntilRef.current > Date.now()) return;
          if (isMine && !locked) onOpen(booking);
          else {
            enterTip();
            setStickyTip(true);
          }
        }
      }}
    >
      {isMine && !locked && !saving ? (
        <div
          className={`resize-handle left${clampSide === "l" ? " clamped" : ""}`}
          onPointerDown={(ev) => onHandleDown(ev, "l", booking)}
          aria-hidden="true"
        >
          <span className="grip" />
        </div>
      ) : null}
      <div className="bk-title">
        {isMine ? "✓ " : ""}
        {booking.title}
      </div>
      {saving ? (
        <div className="saving-label">
          {Icons.spinner({ size: 13 })}
          <span>儲存中…</span>
        </div>
      ) : (
        <div className="bk-time">{fmtRange(booking.start, booking.end)}</div>
      )}
      <div className="bk-user">{booking.bookedBy === CURRENT_USER ? "我" : booking.bookedBy}</div>
      {isMine && !locked && !saving ? (
        <div
          className={`resize-handle right${clampSide === "r" ? " clamped" : ""}`}
          onPointerDown={(ev) => onHandleDown(ev, "r", booking)}
          aria-hidden="true"
        >
          <span className="grip" />
        </div>
      ) : null}
      {flagged ? (
        <span className="sr-only">時間衝突</span>
      ) : null}
      {showTip ? (
        <div className={`bk-tip${flagged ? " error" : ""}`} style={{ position: "fixed", left: tipPos.left, top: tipPos.top, transform: "translateY(-100%)" }}>
          <div className="tip-title">{booking.title}</div>
          <div className="tip-row">{fmtDateShort(booking.date)} {fmtRange(booking.start, booking.end)}</div>
          <div className="tip-row">預約人：{booking.bookedBy === CURRENT_USER ? "我" : booking.bookedBy}</div>
          <div className="tip-row">參與人數：{booking.attendees} 人</div>
        </div>
      ) : null}
    </div>
  );
};

const TimelineView = ({ bookings, filter, setFilter, now, onCreateSlot, onOpenEdit, onPatchBookingTime, pushToast, savingId }) => {
  const isMobile = useIsMobile();
  const date = filter.date;
  const visibleRooms = useMemo(() => ROOMS.filter((r) => matchFilters(r, filter)), [filter]);
  const [mobileRoomId, setMobileRoomId] = useState(null);
  const [sel, setSel] = useState(null);
  const [drag, setDrag] = useState(null);
  const dragRef = useRef(null);
  const suppressClickRef = useRef(false);
  const clickSuppressUntilRef = useRef(0);
  const [revertPill, setRevertPill] = useState(null);

  const nowMin = now.getHours() * 60 + now.getMinutes();
  const showNow = date === todayStr() && nowMin >= DAY_START_MIN && nowMin <= DAY_END_MIN;

  const dayBookings = bookings.filter((b) => b.date === date);
  const flaggedIds = drag && drag.overlapHits.length ? drag.overlapHits.map((b) => b.id) : [];

  const slotSelectable = (roomId, i) => {
    if (date < todayStr()) return false;
    const sm = slotStartMin(i);
    if (date === todayStr() && sm < nowMin) return false;
    return !dayBookings.some((b) => {
      if (b.roomId !== roomId) return false;
      const bs = timeToMin(b.start);
      const be = timeToMin(b.end);
      return bs <= sm && be >= sm + SLOT_MIN;
    });
  };

  // ---- slot drag-select (R-02) ----
  const startSlotSelect = (ev, roomId, i, railEl) => {
    if (ev.button !== 0) return;
    if (!slotSelectable(roomId, i)) return;
    const rect = railEl.getBoundingClientRect();
    const slotW = rect.width / SLOTS_PER_DAY;
    const state = { roomId, from: i, to: i, moved: false, rect, slotW };
    const onMove = (me) => {
      let idx = Math.floor((me.clientX - rect.left) / slotW);
      idx = clampNum(idx, 0, SLOTS_PER_DAY - 1);
      if (idx === state.from && !state.moved) return;
      let from = state.from;
      let to = state.from;
      if (idx > from) {
        for (let k = from; k <= idx; k++) {
          if (!slotSelectable(roomId, k)) break;
          to = k;
        }
      } else {
        for (let k = from; k >= idx; k--) {
          if (!slotSelectable(roomId, k)) break;
          from = k;
        }
      }
      state.to = idx >= from ? to : from;
      state.lo = Math.min(from, to);
      state.hi = Math.max(from, to);
      if (state.hi > state.lo) state.moved = true;
      setSel({ roomId, lo: state.lo, hi: state.hi });
    };
    const onUp = () => {
      window.removeEventListener("pointermove", onMove);
      window.removeEventListener("pointerup", onUp);
      if (state.moved) {
        suppressClickRef.current = true;
        setTimeout(() => (suppressClickRef.current = false), 150);
        setSel(null);
        onCreateSlot({
          roomId,
          date,
          start: minToTime(slotStartMin(state.lo)),
          end: minToTime(slotStartMin(state.hi) + SLOT_MIN),
        });
      }
    };
    window.addEventListener("pointermove", onMove);
    window.addEventListener("pointerup", onUp);
  };

  const slotClick = (roomId, i) => {
    if (suppressClickRef.current) return;
    if (!slotSelectable(roomId, i)) return;
    onCreateSlot({
      roomId,
      date,
      start: minToTime(slotStartMin(i)),
      end: minToTime(slotStartMin(i) + SLOT_MIN),
    });
  };

  // ---- drag resize engine (R-19~R-22) ----
  const computeLimits = useCallback(
    (booking, side) => {
      const s = timeToMin(booking.start);
      const e = timeToMin(booking.end);
      const others = bookings.filter(
        (b) => b.id !== booking.id && b.roomId === booking.roomId && b.date === booking.date
      );
      const prev = others
        .filter((b) => timeToMin(b.end) <= s)
        .sort((a, b) => timeToMin(b.end) - timeToMin(a.end))[0];
      const next = others
        .filter((b) => timeToMin(b.start) >= e)
        .sort((a, b) => timeToMin(a.start) - timeToMin(b.start))[0];
      const lockNow = booking.date === todayStr();
      const nowCeil = ceilToSlot(nowMin);
      if (side === "l") {
        const cons = [
          { v: DAY_START_MIN, msg: "可預約時間是 08:00–20:00" },
          { v: e - MAX_DURATION_MIN, msg: "每次最長 4 小時" },
        ];
        if (prev) cons.push({ v: timeToMin(prev.end), msg: `前面是「${prev.title}」${fmtRange(prev.start, prev.end)}，不能再提早` });
        if (lockNow && nowCeil > DAY_START_MIN) cons.push({ v: nowCeil, msg: "開始時間不能早於現在" });
        const min = Math.max(...cons.map((c) => c.v));
        const max = e - MIN_DURATION_MIN;
        return { min, max, lowerCons: cons };
      }
      const cons = [
        { v: DAY_END_MIN, msg: "可預約時間是 08:00–20:00" },
        { v: s + MAX_DURATION_MIN, msg: "每次最長 4 小時" },
      ];
      if (next) cons.push({ v: timeToMin(next.start), msg: `後面是「${next.title}」${fmtRange(next.start, next.end)}，不能再延後` });
      const max = Math.min(...cons.map((c) => c.v));
      const min = s + MIN_DURATION_MIN;
      return { min, max, upperCons: cons };
    },
    [bookings, nowMin]
  );

  const onHandleDown = (ev, side, booking) => {
    ev.preventDefault();
    ev.stopPropagation();
    if (savingId) return;
    const cardEl = ev.currentTarget.closest(".bk-card");
    const railEl = cardEl.parentElement;
    const rect = railEl.getBoundingClientRect();
    const axis = isMobile ? "y" : "x";
    const pxPerMin = (axis === "x" ? rect.width : rect.height) / (DAY_END_MIN - DAY_START_MIN);
    const limits = computeLimits(booking, side);
    const st = {
      id: booking.id,
      side,
      axis,
      pos0: axis === "x" ? ev.clientX : ev.clientY,
      orig: { start: timeToMin(booking.start), end: timeToMin(booking.end) },
      limits,
      pxPerMin,
      rect,
    };
    dragRef.current = st;
    setDrag({
      id: booking.id,
      roomId: booking.roomId,
      date: booking.date,
      title: booking.title,
      side,
      axis,
      curStart: st.orig.start,
      curEnd: st.orig.end,
      clampMsg: null,
      overlapHits: [],
    });
  };

  useEffect(() => {
    if (!drag) return;
    const st = dragRef.current;

    const move = (me) => {
      const pos = st.axis === "x" ? me.clientX : me.clientY;
      const raw = snapToSlot((st.side === "l" ? st.orig.start : st.orig.end) + (pos - st.pos0) / st.pxPerMin);
      let curStart = st.orig.start;
      let curEnd = st.orig.end;
      let clampMsg = null;
      if (st.side === "l") {
        const v = clampNum(raw, st.limits.min, st.limits.max);
        if (raw < st.limits.min) {
          const hit = st.limits.lowerCons.find((c) => c.v === st.limits.min);
          clampMsg = hit ? hit.msg : null;
        } else if (raw > st.limits.max) {
          clampMsg = "每次最短 30 分鐘";
        }
        curStart = v;
      } else {
        const v = clampNum(raw, st.limits.min, st.limits.max);
        if (raw > st.limits.max) {
          const hit = st.limits.upperCons.find((c) => c.v === st.limits.max);
          clampMsg = hit ? hit.msg : null;
        } else if (raw < st.limits.min) {
          clampMsg = "每次最短 30 分鐘";
        }
        curEnd = v;
      }
      const cand = {
        date: drag.date,
        start: minToTime(curStart),
        end: minToTime(curEnd),
      };
      const overlapHits = findUserConflicts(bookings, { ...cand, roomId: drag.roomId }, CURRENT_USER, drag.id);
      setDrag((d) => ({ ...d, curStart, curEnd, clampMsg, overlapHits }));
    };

    const up = () => {
      window.removeEventListener("pointermove", move);
      window.removeEventListener("pointerup", up);
      window.removeEventListener("keydown", esc);
      clickSuppressUntilRef.current = Date.now() + 300;
      const latest = dragLatestRef.current;
      const st0 = dragRef0Ref.current;
      dragRef.current = null;
      if (!latest || !st0) return;
      setDrag(null);
      if (latest.overlapHits.length) {
        const left = st0.rect.left + pctOfMin(st0.orig.start) / 100 * st0.rect.width;
        setRevertPill({
          left: clampNum(left, st0.rect.left, Math.max(st0.rect.left, st0.rect.right - 240)),
          top: clampNum(st0.rect.top + st0.rect.height * 0.45, 60, window.innerHeight - 60),
          text: `回到原本的 ${minToTime(st0.orig.start)}–${minToTime(st0.orig.end)}`,
        });
        setTimeout(() => setRevertPill(null), 1600);
        pushToast({ kind: "warn", title: "時間沒有變更", body: "你在這個時段已有其他預約" });
        return;
      }
      if (latest.curStart === st0.orig.start && latest.curEnd === st0.orig.end) return;
      onPatchBookingTime(latest.id, minToTime(latest.curStart), minToTime(latest.curEnd));
    };

    const esc = (ke) => {
      if (ke.key === "Escape") {
        ke.preventDefault();
        clickSuppressUntilRef.current = Date.now() + 300;
        window.removeEventListener("pointermove", move);
        window.removeEventListener("pointerup", up);
        window.removeEventListener("keydown", esc);
        dragRef.current = null;
        setDrag(null);
      }
    };

    window.addEventListener("pointermove", move);
    window.addEventListener("pointerup", up);
    window.addEventListener("keydown", esc);
    return () => {
      window.removeEventListener("pointermove", move);
      window.removeEventListener("pointerup", up);
      window.removeEventListener("keydown", esc);
    };
  }, [drag ? drag.id : null]);

  const dragLatestRef = useRef(null);
  const dragRef0Ref = useRef(null);
  dragLatestRef.current = drag;
  if (drag) dragRef0Ref.current = dragRef.current;

  const renderRailBookings = (room, axis) =>
    dayBookings
      .filter((b) => b.roomId === room.id)
      .map((b) => {
        const isMine = b.bookedBy === CURRENT_USER;
        const locked = isLockWindow(b, now.getTime());
        const isDragged = drag && drag.id === b.id;
        const shown = isDragged
          ? { ...b, start: minToTime(drag.curStart), end: minToTime(drag.curEnd) }
          : b;
        return (
          <BookingCard
            key={b.id}
            booking={shown}
            isMine={isMine}
            locked={locked}
            saving={savingId === b.id}
            flagged={flaggedIds.includes(b.id) || (isDragged && drag.overlapHits.length > 0)}
            dragging={Boolean(isDragged)}
            clampSide={isDragged && drag.clampMsg ? drag.side : null}
            axis={axis}
            onOpen={onOpenEdit}
            onHandleDown={onHandleDown}
            suppressUntilRef={clickSuppressUntilRef}
          />
        );
      });

  const nowOverlay = (axis) => {
    if (!showNow) return null;
    const p = pctOfMin(nowMin);
    return axis === "x" ? (
      <div className="now-line" style={{ left: `${p}%` }} />
    ) : (
      <div className="m-now-line" style={{ top: `${p}%` }} />
    );
  };

  const hourLabels = () =>
    Array.from({ length: 12 }, (_, k) => 8 + k).map((h) => (
      <span key={h} className="hour-label" style={{ left: `${((h - 8) * 60) / 720 * 100}%` }}>
        {pad2(h)}:00
      </span>
    ));

  const filterBar = (
    <div className="filter-bar">
      <button type="button" className="btn-icon" aria-label="前一天" onClick={() => setFilter({ ...filter, date: addDaysStr(date, -1) })}>
        {Icons.chevronLeft({ size: 15 })}
      </button>
      <div className="date-pill">
        {Icons.calendar({ size: 15 })}
        <input type="date" value={date} aria-label="日期" onChange={(e) => e.target.value && setFilter({ ...filter, date: e.target.value })} />
        <span className="weekday">({weekdayLabel(date)})</span>
      </div>
      <button type="button" className="btn-icon" aria-label="後一天" onClick={() => setFilter({ ...filter, date: addDaysStr(date, 1) })}>
        {Icons.chevronRight({ size: 15 })}
      </button>
      <button type="button" className="btn" onClick={() => setFilter({ ...filter, date: todayStr() })}>今天</button>
      <span className="filter-label">參與人數</span>
      <select className="select" aria-label="參與人數" value={filter.party} onChange={(e) => setFilter({ ...filter, party: Number(e.target.value) })}>
        <option value={0}>不限</option>
        {[2, 4, 6, 8, 10, 16, 30].map((n) => (
          <option key={n} value={n}>{n}</option>
        ))}
      </select>
      <span className="filter-label">設備</span>
      {EQUIPMENT_OPTIONS.map((eq) => (
        <button
          key={eq}
          type="button"
          className={`equip-chip${filter.equip.includes(eq) ? " on" : ""}`}
          aria-pressed={filter.equip.includes(eq)}
          onClick={() =>
            setFilter({
              ...filter,
              equip: filter.equip.includes(eq) ? filter.equip.filter((x) => x !== eq) : [...filter.equip, eq],
            })
          }
        >
          {eq}
        </button>
      ))}
    </div>
  );

  const legend = (
    <div className="tl-legend">
      <span className="legend-item"><span className="legend-swatch" />可預約</span>
      <span className="legend-item"><span className="legend-swatch booked" />已被預約</span>
      <span className="legend-item"><span className="legend-swatch mine" />✓ 我的預約</span>
      <span className="legend-item"><span className="legend-swatch past" />已過去</span>
      <span className="legend-item"><span className="legend-now" />目前時間</span>
      <span className="legend-right">每格 30 分鐘</span>
    </div>
  );

  const emptyFilters = (
    <div className="empty-state">
      <span className="empty-icon">{Icons.search({ size: 24 })}</span>
      <h3>沒有符合條件的會議室</h3>
      <p>試著減少設備條件，或調低參與人數。</p>
      <button type="button" className="btn" onClick={() => setFilter({ date, party: 0, equip: [] })}>清除篩選</button>
    </div>
  );

  // overlays while dragging (fixed so scroll containers don't clip)
  const dragOverlays = () => {
    if (!drag) return null;
    const st = dragRef.current || dragRef0Ref.current;
    if (!st) return null;
    const xOf = (m) => st.rect.left + (pctOfMin(m) / 100) * st.rect.width;
    const yOf = (m) => st.rect.top + (pctOfMin(m) / 100) * st.rect.height;
    const tipX = st.axis === "x" ? xOf(drag.curStart) : xOf(DAY_START_MIN);
    const rawTipY = st.axis === "x" ? st.rect.top - 4 : yOf(drag.curStart) - 4;
    const tipY = st.axis === "x" ? rawTipY : clampNum(rawTipY, 56, window.innerHeight - 20);
    const dur = fmtDuration(minToTime(drag.curStart), minToTime(drag.curEnd));
    const overlapHit = drag.overlapHits[0];
    const chipX = xOf(st.side === "l" ? drag.curStart : drag.curEnd) - 8;
    const chipY = st.axis === "x" ? st.rect.top - 34 : yOf(st.side === "l" ? drag.curStart : drag.curEnd) - 30;
    return (
      <React.Fragment>
        <div className={`bk-tip${drag.overlapHits.length ? " error" : ""}`} style={{ left: clampNum(tipX, 8, window.innerWidth - 280), top: tipY, transform: "translateY(-100%)" }}>
          <div className="tip-title">{minToTime(drag.curStart)}–{minToTime(drag.curEnd)}（{dur}）</div>
          {overlapHit ? (
            <div className="tip-row">你在 {getRoomById(overlapHit.roomId).name} 已有「{overlapHit.title}」{fmtRange(overlapHit.start, overlapHit.end)}</div>
          ) : null}
        </div>
        {drag.clampMsg ? (
          <div className="drag-chip" style={{ left: clampNum(chipX, 8, window.innerWidth - 320), top: Math.max(8, chipY) }}>
            {drag.clampMsg}
          </div>
        ) : null}
      </React.Fragment>
    );
  };

  if (!visibleRooms.length) {
    return (
      <div>
        {filterBar}
        {emptyFilters}
      </div>
    );
  }

  if (isMobile) {
    const activeRoom = visibleRooms.find((r) => r.id === mobileRoomId) || visibleRooms[0];
    return (
      <div>
        {filterBar}
        <div className="m-room-chips" role="tablist" aria-label="選擇會議室">
          {visibleRooms.map((r) => (
            <button
              key={r.id}
              type="button"
              role="tab"
              aria-selected={r.id === activeRoom.id}
              className={`m-room-chip${r.id === activeRoom.id ? " on" : ""}`}
              onClick={() => setMobileRoomId(r.id)}
            >
              {r.name}
            </button>
          ))}
        </div>
        <div className="m-rail-wrap">
          <div className="m-room-title">
            {activeRoom.name} · {roomMetaLine(activeRoom)} · {fmtDateLong(date)}
          </div>
          <div className="m-rail" style={{ height: SLOTS_PER_DAY * 52 }}>
            {Array.from({ length: SLOTS_PER_DAY }, (_, i) => {
              const past = date < todayStr() || (date === todayStr() && slotStartMin(i) < nowMin);
              const booked = dayBookings.some((b) => b.roomId === activeRoom.id && timeToMin(b.start) <= slotStartMin(i) && timeToMin(b.end) >= slotStartMin(i) + SLOT_MIN);
              return (
                <button
                  key={i}
                  type="button"
                  className={`m-slot${i % 2 === 0 ? " hour-start" : ""}${past ? " past" : ""}`}
                  style={{ top: `${(i / SLOTS_PER_DAY) * 100}%`, height: `${100 / SLOTS_PER_DAY}%` }}
                  disabled={past || booked}
                  aria-label={`${minToTime(slotStartMin(i))} 空檔，預約 ${activeRoom.name}`}
                  onClick={() => slotClick(activeRoom.id, i)}
                >
                  <span className="m-slot-time">{minToTime(slotStartMin(i))}</span>
                </button>
              );
            })}
            {nowOverlay("y")}
            {showNow ? <span className="m-now-badge" style={{ top: `${pctOfMin(nowMin)}%` }}>{minToTime(nowMin)}</span> : null}
            {renderRailBookings(activeRoom, "y")}
          </div>
        </div>
        {legend}
        {dragOverlays()}
        {revertPill ? (
          <div className="revert-pill" style={{ left: revertPill.left, top: revertPill.top }}>
            {Icons.arrowLR({ size: 14 })}
            {revertPill.text}
          </div>
        ) : null}
      </div>
    );
  }

  return (
    <div>
      {filterBar}
      <div className="tl-scroll">
        <div className="tl">
          <div className="tl-row head">
            <div className="room-cell"><span className="head-label">會議室</span></div>
            <div className="tl-head-rail">
              {hourLabels()}
              {showNow ? <span className="now-badge" style={{ left: `${pctOfMin(nowMin)}%` }}>{minToTime(nowMin)}</span> : null}
            </div>
          </div>
          {visibleRooms.map((room) => (
            <div className="tl-row" key={room.id}>
              <div className="room-cell">
                <span className="room-name">{room.name}</span>
                <span className="room-meta">{Icons.users({ size: 13 })}{roomMetaLine(room)}</span>
              </div>
              <div className="rail" data-room={room.id}>
                {Array.from({ length: SLOTS_PER_DAY }, (_, i) => {
                  const past = date < todayStr() || (date === todayStr() && slotStartMin(i) < nowMin);
                  const booked = dayBookings.some((b) => b.roomId === room.id && timeToMin(b.start) <= slotStartMin(i) && timeToMin(b.end) >= slotStartMin(i) + SLOT_MIN);
                  const selOn = sel && sel.roomId === room.id && i >= sel.lo && i <= sel.hi;
                  return (
                    <button
                      key={i}
                      type="button"
                      className={`slot-cell${i % 2 === 0 ? " hour-start" : ""}${past ? " past" : ""}${selOn ? " sel-range" : ""}`}
                      style={{ left: `${(i / SLOTS_PER_DAY) * 100}%`, width: `${100 / SLOTS_PER_DAY}%` }}
                      disabled={past || booked}
                      aria-label={past ? `${minToTime(slotStartMin(i))} 已過去` : booked ? `${minToTime(slotStartMin(i))} 已被預約` : `${minToTime(slotStartMin(i))} 空檔，預約 ${room.name}`}
                      onPointerDown={(ev) => startSlotSelect(ev, room.id, i, ev.currentTarget.parentElement)}
                      onClick={() => slotClick(room.id, i)}
                    />
                  );
                })}
                {nowOverlay("x")}
                {sel && sel.roomId === room.id && sel.hi > sel.lo ? (
                  <div
                    className="sel-badge"
                    style={{ left: `${(sel.lo / SLOTS_PER_DAY) * 100}%`, width: `${((sel.hi - sel.lo + 1) / SLOTS_PER_DAY) * 100}%` }}
                  >
                    {minToTime(slotStartMin(sel.lo))}–{minToTime(slotStartMin(sel.hi) + SLOT_MIN)} 拖曳選取中
                  </div>
                ) : null}
                {renderRailBookings(room, "x")}
              </div>
            </div>
          ))}
        </div>
      </div>
      {legend}
      {dragOverlays()}
      {revertPill ? (
        <div className="revert-pill" style={{ left: revertPill.left, top: revertPill.top }}>
          {Icons.arrowLR({ size: 14 })}
          {revertPill.text}
        </div>
      ) : null}
    </div>
  );
};
