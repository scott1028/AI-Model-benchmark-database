// Port of the Settings (KConfigSkeleton "kminesrc") and of the high-score store
// that KGameHighScoreDialog keeps per difficulty.  Browser equivalent: localStorage.
// Keys, defaults and ranges are the ones read out of the binary (report §3.3/§3.6).

const SETTINGS_KEY = 'webgl-kmines.settings';
const SCORES_KEY = 'webgl-kmines.highscores';

export const Mouse = { Release: 0, Press: 1 };

export const DIFFICULTY = {
  Easy: { id: 30, name: 'Easy', height: 9, width: 9, mines: 10 },
  Medium: { id: 40, name: 'Medium', height: 16, width: 16, mines: 40 },
  Hard: { id: 50, name: 'Hard', height: 16, width: 30, mines: 99 },
  Custom: { id: -1, name: 'Custom' },
};

export const DIFFICULTY_ORDER = [DIFFICULTY.Easy, DIFFICULTY.Medium, DIFFICULTY.Hard, DIFFICULTY.Custom];

export const DEFAULTS = {
  difficultyId: DIFFICULTY.Easy.id,
  useQuestionMarks: true,          // kcfg_UseQuestionMarks, default true
  allowKminesReset: true,          // kcfg_AllowKminesReset, default true
  disableScoreOnReset: false,      // kcfg_DisableScoreOnReset, default false
  exploreWithLeftClickOnNumberCells: false,
  placeFlagOn: Mouse.Release,      // kcfg_PlaceFlagOn: MouseRelease | MousePress
  customWidth: 10,                 // kcfg_CustomWidth, 5..50
  customHeight: 10,                // kcfg_CustomHeight, 5..50
  customMines: 20,                 // kcfg_CustomMines, 1..w*h-10
  theme: 'default',
};

const clamp = (value, min, max) => Math.min(max, Math.max(min, value));

const isValid = (settings) =>
  settings && typeof settings.useQuestionMarks === 'boolean' &&
  typeof settings.customWidth === 'number';

export const loadSettings = () => {
  let stored = null;
  try {
    stored = JSON.parse(localStorage.getItem(SETTINGS_KEY));
  } catch (error) {
    stored = null;
  }
  return { ...DEFAULTS, ...(isValid(stored) ? stored : {}) };
};

export const saveSettings = (settings) => {
  try {
    localStorage.setItem(SETTINGS_KEY, JSON.stringify(settings));
  } catch (error) {
    // storage may be unavailable (private mode); the game still runs
  }
};

export const customLimits = {
  width: { min: 5, max: 50 },
  height: { min: 5, max: 50 },
  minesMin: 1,
  maxMines: (width, height) => Math.max(1, width * height - 10),   // CustomGameConfig::updateMaxMines
};

export const clampSettings = (settings) => {
  const width = clamp(Math.round(settings.customWidth), customLimits.width.min, customLimits.width.max);
  const height = clamp(Math.round(settings.customHeight), customLimits.height.min, customLimits.height.max);
  const mines = clamp(Math.round(settings.customMines), customLimits.minesMin, customLimits.maxMines(width, height));
  return { ...settings, customWidth: width, customHeight: height, customMines: mines };
};

export const loadScores = () => {
  let stored = null;
  try {
    stored = JSON.parse(localStorage.getItem(SCORES_KEY));
  } catch (error) {
    stored = null;
  }
  return stored && typeof stored === 'object' ? stored : {};
};

// KGameClock::seconds() is the score, so a smaller value wins; keep 10 per level.
export const addScore = (difficultyId, seconds) => {
  const scores = loadScores();
  const list = scores[difficultyId] || [];
  const entry = { seconds, date: new Date().toISOString().slice(0, 10) };
  list.push(entry);
  list.sort((a, b) => a.seconds - b.seconds);
  const trimmed = list.slice(0, 10);
  scores[difficultyId] = trimmed;
  try {
    localStorage.setItem(SCORES_KEY, JSON.stringify(scores));
  } catch (error) {
    // ignore persistence failures
  }
  return trimmed.includes(entry);
};

export const formatTime = (seconds) => {
  const total = Math.min(3599, Math.max(0, Math.floor(seconds)));   // KGameClock caps at 59:59
  const mm = String(Math.floor(total / 60)).padStart(2, '0');
  const ss = String(total % 60).padStart(2, '0');
  return `${mm}:${ss}`;
};
