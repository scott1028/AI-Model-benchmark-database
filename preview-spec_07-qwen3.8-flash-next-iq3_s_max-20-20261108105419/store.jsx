// Data layer: rooms, seed bookings, localStorage, domain checks, mock API.

const ROOMS = [
  { id: "A101", name: "A101 小會議室", capacity: 4, equipment: ["電視"] },
  { id: "A102", name: "A102 中會議室", capacity: 8, equipment: ["投影機", "白板"] },
  { id: "B201", name: "B201 大會議室", capacity: 16, equipment: ["投影機", "視訊"] },
  { id: "B202", name: "B202 洽談室", capacity: 2, equipment: [] },
  { id: "C301", name: "C301 訓練教室", capacity: 30, equipment: ["投影機", "麥克風"] },
];

const EQUIPMENT_OPTIONS = ["投影機", "白板", "視訊", "電視", "麥克風"];

const STORAGE_KEY = "meeting-booking-spec07-v1";

const buildSeedBookings = () => {
  const t = todayStr();
  const mk = (roomId, dayOffset, start, end, title, attendees, bookedBy, notes) => ({
    id: genId(),
    roomId,
    date: addDaysStr(t, dayOffset),
    start,
    end,
    title,
    attendees,
    bookedBy,
    notes: notes || "",
  });
  return [
    mk("A101", 0, "09:00", "10:00", "晨會", 3, "陳怡君"),
    mk("A102", 0, "10:00", "11:00", "產品週會", 6, "王大問"),
    mk("A102", 0, "14:00", "15:30", "設計評審", 6, CURRENT_USER),
    mk("B201", 0, "13:30", "15:00", "季度規劃", 12, "林志豪"),
    mk("B201", 0, "16:00", "17:00", "客戶簡報", 10, "張雅婷"),
    mk("B202", 0, "13:00", "14:00", "1:1 面談", 2, CURRENT_USER),
    mk("C301", 0, "13:00", "17:00", "新人訓練", 25, "人資部"),
    mk("A102", 1, "11:00", "12:00", "需求討論", 6, CURRENT_USER),
    mk("B201", 2, "10:00", "11:00", "Sprint 回顧", 10, CURRENT_USER),
    mk("A101", 7, "15:00", "16:00", "面試", 3, CURRENT_USER),
  ];
};

const saveBookings = (bookings) => {
  try {
    localStorage.setItem(STORAGE_KEY, JSON.stringify({ version: 1, bookings }));
  } catch (e) {
    // storage unavailable; in-memory only
  }
};

const loadBookings = () => {
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    if (raw) {
      const data = JSON.parse(raw);
      if (data && data.version === 1 && Array.isArray(data.bookings)) return data.bookings;
    }
  } catch (e) {
    // fall through to seed
  }
  const seeded = buildSeedBookings();
  saveBookings(seeded);
  return seeded;
};

const getRoomById = (id) => ROOMS.find((r) => r.id === id);

const roomSummary = (room) =>
  `${room.name} · ${room.capacity} 人${room.equipment.length ? ` · ${room.equipment.join("、")}` : ""}`;

const roomMetaLine = (room) =>
  room.equipment.length ? `${room.capacity} 人 · ${room.equipment.join("、")}` : `${room.capacity} 人`;

const bookingStartTs = (b) => parseDateStr(b.date).getTime() + timeToMin(b.start) * 60000;

const bookingEndTs = (b) => parseDateStr(b.date).getTime() + timeToMin(b.end) * 60000;

const isEnded = (b, nowTs) => bookingEndTs(b) <= nowTs;

// Within 30 minutes before start (already-started included) -> cannot edit or cancel.
const isLockWindow = (b, nowTs) => bookingStartTs(b) - nowTs <= 30 * 60000;

const sameDayOverlap = (a, b) =>
  a.date === b.date && isTimeOverlap(a.start, a.end, b.start, b.end);

const findRoomConflicts = (bookings, cand, excludeId) =>
  bookings.filter(
    (b) => b.id !== excludeId && b.roomId === cand.roomId && sameDayOverlap(b, cand)
  );

const findUserConflicts = (bookings, cand, user, excludeId) =>
  bookings.filter(
    (b) => b.id !== excludeId && b.bookedBy === user && sameDayOverlap(b, cand)
  );

// Rooms free at the candidate slot with capacity >= attendees, ascending capacity, max 3.
const suggestRooms = (bookings, cand, attendees, excludeRoomId) =>
  ROOMS.filter((r) => r.id !== excludeRoomId && r.capacity >= attendees)
    .filter((r) => findRoomConflicts(bookings, { ...cand, roomId: r.id }, cand.id).length === 0)
    .sort((a, b) => a.capacity - b.capacity)
    .slice(0, 3);

const matchFilters = (room, filter) =>
  (filter.party === 0 || room.capacity >= filter.party) &&
  filter.equip.every((e) => room.equipment.includes(e));

// Mock API: ~800ms latency; titles containing [500] fail like a server error.
const simulateApi = (payload) =>
  new Promise((resolve, reject) => {
    setTimeout(() => {
      if (String(payload.title).includes("[500]")) reject(new Error("server-error"));
      else resolve(payload);
    }, 800);
  });
