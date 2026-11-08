// Booking form modal: create and edit modes, validation, conflict UI, mock API.

const START_OPTIONS = Array.from({ length: SLOTS_PER_DAY }, (_, i) => minToTime(DAY_START_MIN + i * SLOT_MIN));
const END_OPTIONS = Array.from({ length: SLOTS_PER_DAY }, (_, i) => minToTime(DAY_START_MIN + (i + 1) * SLOT_MIN));
const DATE_OPTIONS = Array.from({ length: BOOKING_WINDOW_DAYS }, (_, i) => addDaysStr(todayStr(), i));

const BookingModal = ({ mode, initial, bookings, onClose, onCommit, pushToast }) => {
  const editId = mode === "edit" ? initial.id : null;
  const [roomId, setRoomId] = useState(initial.roomId);
  const [date, setDate] = useState(initial.date);
  const [start, setStart] = useState(initial.start);
  const [end, setEnd] = useState(initial.end);
  const [title, setTitle] = useState(initial.title || "");
  const [attendees, setAttendees] = useState(String(initial.attendees || 6));
  const [notes, setNotes] = useState(initial.notes || "");
  const [errors, setErrors] = useState({});
  const [conflict, setConflict] = useState(null);
  const [submitting, setSubmitting] = useState(false);

  const room = getRoomById(roomId);

  const formValues = () => ({
    roomId,
    date,
    start,
    end,
    title: title.trim(),
    attendees: Number(attendees),
    notes,
  });

  const isDirty =
    mode !== "edit" ||
    roomId !== initial.roomId ||
    date !== initial.date ||
    start !== initial.start ||
    end !== initial.end ||
    title.trim() !== (initial.title || "") ||
    Number(attendees) !== initial.attendees ||
    notes !== (initial.notes || "");

  const validate = () => {
    const errs = {};
    const trimmed = title.trim();
    if (!trimmed) errs.title = "請輸入會議主題";
    else if (trimmed.length > 50) errs.title = "會議主題最多 50 字";

    if (DATE_OPTIONS.indexOf(date) === -1) errs.date = "日期限今天起 14 天內（含今天）";

    const s = timeToMin(start);
    const e = timeToMin(end);
    if (s < DAY_START_MIN || e > DAY_END_MIN) errs.time = "可預約時間是 08:00–20:00";
    else if (e <= s) errs.time = "結束時間必須晚於開始時間";
    else if (e - s < MIN_DURATION_MIN) errs.time = "每次最短 30 分鐘";
    else if (e - s > MAX_DURATION_MIN) errs.time = "每次最長 4 小時";
    else if (date === todayStr()) {
      const now = new Date();
      if (s < now.getHours() * 60 + now.getMinutes()) errs.time = "開始時間不能早於現在";
    }

    const n = Number(attendees);
    if (!attendees.trim() || !Number.isInteger(n)) errs.attendees = "請輸入整數人數";
    else if (n < 1) errs.attendees = "參與人數至少 1 人";
    else if (n > room.capacity) errs.attendees = `參與人數超過會議室容量（${room.capacity} 人）`;

    if (notes.length > 200) errs.notes = "備註最多 200 字";
    return errs;
  };

  const checkConflicts = () => {
    const cand = { roomId, date, start, end };
    const roomHits = findRoomConflicts(bookings, cand, editId);
    if (roomHits.length) return { roomHits, userHits: [] };
    const userHits = findUserConflicts(bookings, cand, CURRENT_USER, editId);
    return userHits.length ? { roomHits: [], userHits } : null;
  };

  const stepAttendees = (delta) => {
    const n = Number(attendees);
    const base = Number.isInteger(n) ? n : 1;
    setAttendees(String(clampNum(base + delta, 1, room.capacity)));
  };

  const handleSubmit = async (ev) => {
    ev.preventDefault();
    if (submitting) return;
    const errs = validate();
    setErrors(errs);
    setConflict(null);
    if (Object.keys(errs).length) return;

    setSubmitting(true);
    try {
      await simulateApi({ title: title.trim() });
    } catch (e) {
      setSubmitting(false);
      const c = checkConflicts();
      if (c) setConflict(c);
      else pushToast({ kind: "error", title: "預約失敗，請稍後再試" });
      return;
    }
    const c = checkConflicts();
    if (c) {
      setSubmitting(false);
      setConflict(c);
      return;
    }
    setSubmitting(false);
    onCommit({ ...formValues(), id: editId || genId(), bookedBy: CURRENT_USER });
    onClose();
  };

  const pickSuggested = (r) => {
    setRoomId(r.id);
    setConflict(null);
    const next = { ...errors };
    delete next.time;
    setErrors(next);
  };

  const suggestions =
    conflict && conflict.roomHits.length
      ? suggestRooms(bookings, { roomId, date, start, end }, Number(attendees) || 1, roomId)
      : [];
  const timeInvalid = Boolean(errors.time) || Boolean(conflict);

  const fieldError = (key) =>
    errors[key] ? (
      <div className="field-error" id={`err-${key}`} role="alert">
        {Icons.warn({ size: 14 })}
        <span>{errors[key]}</span>
      </div>
    ) : null;

  const submittingLabel = mode === "edit" ? "儲存中…" : "預約中…";

  return (
    <Modal title={mode === "edit" ? "修改預約" : "新增預約"} labelledBy="booking-modal-title" onClose={onClose}>
      <form onSubmit={handleSubmit} noValidate>
        <div className={`modal-body${submitting ? " form-disabled" : ""}`}>
          {conflict && conflict.roomHits.length ? (
            <div className="form-banner error" role="alert">
              {Icons.warn({ size: 16 })}
              <div className="banner-body">
                {conflict.roomHits.map((b) => (
                  <b key={b.id}>
                    此時段已被預約：{getRoomById(b.roomId).id} {fmtRange(b.start, b.end)} {b.title}（{b.bookedBy}）
                  </b>
                ))}
                <span>請調整時間，或改預約下方同時段可用的會議室。</span>
              </div>
            </div>
          ) : null}
          {conflict && conflict.userHits.length
            ? conflict.userHits.map((b) => (
                <div className="form-banner error" role="alert" key={b.id}>
                  {Icons.warn({ size: 16 })}
                  <div className="banner-body">
                    <b>
                      你在這個時段已經有預約：{getRoomById(b.roomId).name} {fmtRange(b.start, b.end)} {b.title}
                    </b>
                  </div>
                </div>
              ))
            : null}

          <div className="form-field">
            <span className="form-label" id="lbl-room">會議室</span>
            <div className="room-locked" aria-labelledby="lbl-room">
              <span>{roomSummary(room)}</span>
              {Icons.lock({ size: 15 })}
            </div>
          </div>

          <div className="form-field">
            <label className="form-label" htmlFor="f-date">日期</label>
            <select
              id="f-date"
              className={`form-select${errors.date ? " invalid" : ""}`}
              value={date}
              disabled={submitting}
              aria-invalid={Boolean(errors.date)}
              onChange={(e) => {
                setDate(e.target.value);
                setConflict(null);
              }}
            >
              {DATE_OPTIONS.map((d) => (
                <option key={d} value={d}>{fmtDateLong(d)}</option>
              ))}
            </select>
            {fieldError("date")}
          </div>

          <div className="form-row">
            <div className="form-field">
              <label className="form-label" htmlFor="f-start">開始時間</label>
              <select
                id="f-start"
                className={`form-select${timeInvalid ? " invalid" : ""}`}
                value={start}
                disabled={submitting}
                aria-invalid={timeInvalid}
                onChange={(e) => {
                  setStart(e.target.value);
                  setConflict(null);
                }}
              >
                {START_OPTIONS.map((t) => <option key={t} value={t}>{t}</option>)}
              </select>
            </div>
            <div className="form-field">
              <label className="form-label" htmlFor="f-end">結束時間</label>
              <select
                id="f-end"
                className={`form-select${timeInvalid ? " invalid" : ""}`}
                value={end}
                disabled={submitting}
                aria-invalid={timeInvalid}
                onChange={(e) => {
                  setEnd(e.target.value);
                  setConflict(null);
                }}
              >
                {END_OPTIONS.map((t) => <option key={t} value={t}>{t}</option>)}
              </select>
            </div>
          </div>
          {fieldError("time")}
          {!errors.time && !conflict ? <div className="field-hint">共 {fmtDuration(start, end)}</div> : null}

          <div className="form-field">
            <label className="form-label" htmlFor="f-title">會議主題</label>
            <input
              id="f-title"
              className={`form-input${errors.title ? " invalid" : ""}`}
              data-autofocus
              value={title}
              maxLength={50}
              placeholder="例如：需求討論"
              disabled={submitting}
              aria-invalid={Boolean(errors.title)}
              aria-describedby={errors.title ? "err-title" : undefined}
              onChange={(e) => setTitle(e.target.value)}
            />
            {fieldError("title")}
          </div>

          <div className="form-field">
            <span className="form-label" id="lbl-att">參與人數</span>
            <div className="stepper" aria-labelledby="lbl-att">
              <button type="button" className="step-btn" onClick={() => stepAttendees(-1)} disabled={submitting} aria-label="減少人數">
                {Icons.minus({ size: 14 })}
              </button>
              <input
                className={errors.attendees ? "invalid" : ""}
                value={attendees}
                inputMode="numeric"
                disabled={submitting}
                aria-invalid={Boolean(errors.attendees)}
                aria-describedby={errors.attendees ? "err-attendees" : undefined}
                onChange={(e) => setAttendees(e.target.value.replace(/[^\d]/g, ""))}
              />
              <button type="button" className="step-btn" onClick={() => stepAttendees(1)} disabled={submitting} aria-label="增加人數">
                {Icons.plus({ size: 14 })}
              </button>
              <span className="unit">人，上限 {room.capacity} 人</span>
            </div>
            {fieldError("attendees")}
          </div>

          <div className="form-field">
            <label className="form-label" htmlFor="f-notes">備註 <span className="opt">（選填）</span></label>
            <textarea
              id="f-notes"
              className={`form-input${errors.notes ? " invalid" : ""}`}
              value={notes}
              maxLength={200}
              placeholder="要準備的設備、訪客資訊…"
              disabled={submitting}
              aria-invalid={Boolean(errors.notes)}
              onChange={(e) => setNotes(e.target.value)}
            />
            <div className="counter">{notes.length}/200</div>
            {fieldError("notes")}
          </div>

          {conflict && conflict.roomHits.length ? (
            <div className="suggest-section">
              <h4>同時段可用的會議室</h4>
              <div className="suggest-sub">
                {fmtDateShort(date)} {fmtRange(start, end)} · {Number(attendees) || 0} 人以上
              </div>
              {suggestions.length
                ? suggestions.map((r) => (
                    <div className="suggest-card" key={r.id}>
                      <div>
                        <div className="s-name">{r.name}</div>
                        <div className="s-meta">{roomMetaLine(r)}</div>
                      </div>
                      <button type="button" className="btn" onClick={() => pickSuggested(r)} disabled={submitting}>
                        改預約這間
                      </button>
                    </div>
                  ))
                : <div className="suggest-empty">同時段沒有其他可用的會議室</div>}
            </div>
          ) : null}
        </div>

        <div className="modal-foot">
          <button type="button" className="btn" onClick={onClose} disabled={submitting}>取消</button>
          <button
            type="submit"
            className="btn btn-primary"
            disabled={submitting || (mode === "edit" && !isDirty)}
          >
            {submitting ? Icons.spinner({ size: 15 }) : null}
            {submitting ? submittingLabel : mode === "edit" ? "儲存變更" : "確認預約"}
          </button>
        </div>
      </form>
    </Modal>
  );
};
