// Shared constants and helpers. Loaded as a global scope script (no imports).

const DAY_START_MIN = 8 * 60;
const DAY_END_MIN = 20 * 60;
const SLOT_MIN = 30;
const SLOTS_PER_DAY = (DAY_END_MIN - DAY_START_MIN) / SLOT_MIN;
const MIN_DURATION_MIN = 30;
const MAX_DURATION_MIN = 4 * 60;
const BOOKING_WINDOW_DAYS = 14;
const CURRENT_USER = "王小明";
const WEEKDAYS = ["日", "一", "二", "三", "四", "五", "六"];

const pad2 = (n) => String(n).padStart(2, "0");

const toDateStr = (d) => `${d.getFullYear()}-${pad2(d.getMonth() + 1)}-${pad2(d.getDate())}`;

const todayStr = () => toDateStr(new Date());

const parseDateStr = (s) => {
  const [y, m, d] = s.split("-").map(Number);
  return new Date(y, m - 1, d);
};

const addDaysStr = (s, n) => {
  const d = parseDateStr(s);
  d.setDate(d.getDate() + n);
  return toDateStr(d);
};

const weekdayLabel = (s) => WEEKDAYS[parseDateStr(s).getDay()];

const fmtDateLong = (s) => `${s} (${weekdayLabel(s)})`;

const fmtDateShort = (s) => {
  const d = parseDateStr(s);
  return `${d.getMonth() + 1}/${d.getDate()} (${weekdayLabel(s)})`;
};

const timeToMin = (t) => {
  const [h, m] = t.split(":").map(Number);
  return h * 60 + m;
};

const minToTime = (m) => `${pad2(Math.floor(m / 60))}:${pad2(m % 60)}`;

const fmtRange = (start, end) => `${start}\u2013${end}`;

const fmtDuration = (start, end) => {
  const len = timeToMin(end) - timeToMin(start);
  if (len < 60) return `${len} 分鐘`;
  return `${len / 60} 小時`;
};

const isTimeOverlap = (aStart, aEnd, bStart, bEnd) =>
  timeToMin(aStart) < timeToMin(bEnd) && timeToMin(bStart) < timeToMin(aEnd);

const clampNum = (v, lo, hi) => Math.min(Math.max(v, lo), hi);

const snapToSlot = (m) => Math.round(m / SLOT_MIN) * SLOT_MIN;

const ceilToSlot = (m) => Math.ceil(m / SLOT_MIN) * SLOT_MIN;

const genId = () => `bk-${Date.now().toString(36)}-${Math.random().toString(36).slice(2, 8)}`;

const parseTimelineQuery = (search) => {
  const p = new URLSearchParams(search);
  const date = p.get("date");
  const party = p.get("party");
  const equip = p.get("equip");
  return {
    date: date && /^\d{4}-\d{2}-\d{2}$/.test(date) ? date : todayStr(),
    party: party && /^\d+$/.test(party) ? Number(party) : 0,
    equip: equip ? equip.split(",").map((s) => s.trim()).filter(Boolean) : [],
  };
};

const buildTimelineQuery = (f) => {
  const p = new URLSearchParams();
  if (f.date !== todayStr()) p.set("date", f.date);
  if (f.party) p.set("party", String(f.party));
  if (f.equip.length) p.set("equip", f.equip.join(","));
  const q = p.toString();
  return q ? `?${q}` : location.pathname;
};
