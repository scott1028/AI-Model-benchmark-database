// My Bookings page (R-11, R-12) + cancel confirmation dialog.

const MyBookingsView = ({ bookings, now, onEdit, onCancel, onGoTimeline }) => {
  const nowTs = now.getTime();
  const upcoming = bookings
    .filter((b) => b.bookedBy === CURRENT_USER && !isEnded(b, nowTs))
    .sort((a, b) => bookingStartTs(a) - bookingStartTs(b));

  return (
    <div className="panel">
      <h1 className="page-title">我的預約</h1>
      <div className="my-tabs">
        <button type="button" className="my-tab active" aria-current="true">
          即將來臨<span className="count">{upcoming.length}</span>
        </button>
      </div>
      {upcoming.length ? (
        <table className="bk-table">
          <thead>
            <tr>
              <th>日期</th>
              <th>時間</th>
              <th>會議室</th>
              <th>主題</th>
              <th>參與人數</th>
              <th>操作</th>
            </tr>
          </thead>
          <tbody>
            {upcoming.map((b) => {
              const locked = isLockWindow(b, nowTs);
              return (
                <tr key={b.id}>
                  <td data-label="日期">{fmtDateShort(b.date)}</td>
                  <td data-label="時間">{fmtRange(b.start, b.end)}</td>
                  <td data-label="會議室">{getRoomById(b.roomId).name}</td>
                  <td data-label="主題">{b.title}</td>
                  <td data-label="參與人數">{b.attendees} 人</td>
                  <td data-label="操作">
                    <div className="actions-cell">
                      <div className="actions-row">
                        <button
                          type="button"
                          className="btn btn-outline-danger"
                          disabled={locked}
                          onClick={() => onCancel(b)}
                        >
                          取消預約
                        </button>
                        <button
                          type="button"
                          className="btn btn-outline-primary"
                          disabled={locked}
                          onClick={() => onEdit(b)}
                        >
                          修改
                        </button>
                      </div>
                      {locked ? <span className="lock-note">開始前 30 分鐘內無法修改或取消</span> : null}
                    </div>
                  </td>
                </tr>
              );
            })}
          </tbody>
        </table>
      ) : (
        <div className="empty-state">
          <span className="empty-icon">{Icons.calendar({ size: 24 })}</span>
          <h3>目前沒有預約</h3>
          <p>在時間表上選一個空檔就能預約。</p>
          <button type="button" className="btn btn-primary" onClick={onGoTimeline}>前往預約</button>
        </div>
      )}
    </div>
  );
};

const CancelConfirm = ({ booking, onClose, onConfirm }) => (
  <Modal title="取消預約" labelledBy="cancel-title" onClose={onClose} width="460px">
    <div className="modal-body confirm-body">
      <p>確定要取消「{booking.title}」？此動作無法復原。</p>
      <div className="confirm-box">
        {fmtDateShort(booking.date)} {fmtRange(booking.start, booking.end)} · {getRoomById(booking.roomId).name}
      </div>
    </div>
    <div className="modal-foot">
      <button type="button" className="btn" data-autofocus onClick={onClose}>返回</button>
      <button type="button" className="btn btn-danger" onClick={() => onConfirm(booking)}>確定取消</button>
    </div>
  </Modal>
);
