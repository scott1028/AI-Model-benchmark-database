// Shared UI: inline SVG icons, accessible Modal, toast host.

const { useState, useEffect, useRef, useMemo, useCallback } = React;

const svgBase = {
  fill: "none",
  stroke: "currentColor",
  strokeWidth: 2,
  strokeLinecap: "round",
  strokeLinejoin: "round",
};

const Svg = ({ size = 16, className, children, extra }) => (
  <svg
    width={size}
    height={size}
    viewBox="0 0 24 24"
    className={className}
    aria-hidden="true"
    {...svgBase}
    {...(extra || {})}
  >
    {children}
  </svg>
);

const Icons = {
  calendar: (p) => (
    <Svg {...p}>
      <rect x="3" y="4" width="18" height="18" rx="2" />
      <line x1="16" y1="2" x2="16" y2="6" />
      <line x1="8" y1="2" x2="8" y2="6" />
      <line x1="3" y1="10" x2="21" y2="10" />
    </Svg>
  ),
  chevronLeft: (p) => (
    <Svg {...p}>
      <polyline points="15 18 9 12 15 6" />
    </Svg>
  ),
  chevronRight: (p) => (
    <Svg {...p}>
      <polyline points="9 18 15 12 9 6" />
    </Svg>
  ),
  close: (p) => (
    <Svg {...p}>
      <line x1="18" y1="6" x2="6" y2="18" />
      <line x1="6" y1="6" x2="18" y2="18" />
    </Svg>
  ),
  lock: (p) => (
    <Svg {...p}>
      <rect x="4" y="11" width="16" height="10" rx="2" />
      <path d="M8 11V7a4 4 0 0 1 8 0v4" />
    </Svg>
  ),
  users: (p) => (
    <Svg {...p}>
      <path d="M17 21v-2a4 4 0 0 0-4-4H6a4 4 0 0 0-4 4v2" />
      <circle cx="9.5" cy="7" r="4" />
      <path d="M22 21v-2a4 4 0 0 0-3-3.87" />
      <path d="M16 3.13a4 4 0 0 1 0 7.75" />
    </Svg>
  ),
  plus: (p) => (
    <Svg {...p}>
      <line x1="12" y1="5" x2="12" y2="19" />
      <line x1="5" y1="12" x2="19" y2="12" />
    </Svg>
  ),
  minus: (p) => (
    <Svg {...p}>
      <line x1="5" y1="12" x2="19" y2="12" />
    </Svg>
  ),
  check: (p) => (
    <Svg {...p}>
      <polyline points="20 6 9 17 4 12" />
    </Svg>
  ),
  checkCircle: (p) => (
    <Svg {...p}>
      <circle cx="12" cy="12" r="9" />
      <polyline points="8.5 12.5 11 15 16 9.5" />
    </Svg>
  ),
  xCircle: (p) => (
    <Svg {...p}>
      <circle cx="12" cy="12" r="9" />
      <line x1="9" y1="9" x2="15" y2="15" />
      <line x1="15" y1="9" x2="9" y2="15" />
    </Svg>
  ),
  warn: (p) => (
    <Svg {...p}>
      <path d="M10.3 3.8 1.8 18a2 2 0 0 0 1.7 3h17a2 2 0 0 0 1.7-3L13.7 3.8a2 2 0 0 0-3.4 0z" />
      <line x1="12" y1="9" x2="12" y2="13" />
      <line x1="12" y1="17" x2="12.01" y2="17" />
    </Svg>
  ),
  infoCircle: (p) => (
    <Svg {...p}>
      <circle cx="12" cy="12" r="9" />
      <line x1="12" y1="11" x2="12" y2="16" />
      <line x1="12" y1="8" x2="12.01" y2="8" />
    </Svg>
  ),
  search: (p) => (
    <Svg {...p}>
      <circle cx="11" cy="11" r="7" />
      <line x1="21" y1="21" x2="16.5" y2="16.5" />
    </Svg>
  ),
  arrowLR: (p) => (
    <Svg {...p}>
      <polyline points="8 7 3 12 8 17" />
      <polyline points="16 7 21 12 16 17" />
      <line x1="3" y1="12" x2="21" y2="12" />
    </Svg>
  ),
  spinner: (p) => (
    <Svg {...p} extra={{ className: `${p.className || ""} spinner` }}>
      <path d="M21 12a9 9 0 1 1-6.2-8.6" />
    </Svg>
  ),
};

const Modal = ({ title, titleId, onClose, children, width, labelledBy }) => {
  const panelRef = React.useRef(null);

  React.useEffect(() => {
    const prevFocused = document.activeElement;
    const FOCUSABLE =
      'button, [href], input, select, textarea, [tabindex]:not([tabindex="-1"])';
    const focusIn = () => {
      const el = panelRef.current
        ? panelRef.current.querySelector("[data-autofocus]") ||
          panelRef.current.querySelector(FOCUSABLE)
        : null;
      if (el) el.focus();
      else if (panelRef.current) panelRef.current.focus();
    };
    focusIn();
    const onKey = (e) => {
      if (e.key === "Escape") {
        e.preventDefault();
        e.stopPropagation();
        onClose();
        return;
      }
      if (e.key !== "Tab" || !panelRef.current) return;
      const focusables = Array.from(panelRef.current.querySelectorAll(FOCUSABLE)).filter(
        (el) => !el.disabled
      );
      if (!focusables.length) return;
      const first = focusables[0];
      const last = focusables[focusables.length - 1];
      const active = document.activeElement;
      if (e.shiftKey && (active === first || !panelRef.current.contains(active))) {
        e.preventDefault();
        last.focus();
      } else if (!e.shiftKey && (active === last || !panelRef.current.contains(active))) {
        e.preventDefault();
        first.focus();
      }
    };
    document.addEventListener("keydown", onKey, true);
    return () => {
      document.removeEventListener("keydown", onKey, true);
      if (prevFocused && typeof prevFocused.focus === "function") prevFocused.focus();
    };
  }, [onClose]);

  return (
    <div className="modal-overlay" onMouseDown={(e) => e.target === e.currentTarget && onClose()}>
      <div
        className="modal-panel"
        ref={panelRef}
        role="dialog"
        aria-modal="true"
        aria-labelledby={labelledBy}
        style={width ? { maxWidth: width } : undefined}
        tabIndex={-1}
      >
        <div className="modal-head">
          <h2 id={labelledBy}>{title}</h2>
          <button type="button" className="btn-icon" onClick={onClose} aria-label="關閉">
            {Icons.close({ size: 18 })}
          </button>
        </div>
        {children}
      </div>
    </div>
  );
};

const ToastHost = ({ toasts, onDismiss }) => (
  <div className="toast-host" role="status" aria-live="polite">
    {toasts.map((t) => (
      <div key={t.id} className={`toast ${t.kind}`} onClick={() => onDismiss(t.id)}>
        <span className="toast-ic">
          {t.kind === "success"
            ? Icons.checkCircle({ size: 20 })
            : t.kind === "error"
              ? Icons.xCircle({ size: 20 })
              : Icons.warn({ size: 20 })}
        </span>
        <div>
          <div className="toast-title">{t.title}</div>
          {t.body ? <div className="toast-body">{t.body}</div> : null}
        </div>
      </div>
    ))}
  </div>
);
