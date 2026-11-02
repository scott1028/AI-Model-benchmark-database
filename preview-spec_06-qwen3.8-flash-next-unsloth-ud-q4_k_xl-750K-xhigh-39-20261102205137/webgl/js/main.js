// Entry point: create the game, or explain why it cannot run.

import { Game } from './game.js';

const showError = (message) => {
  const wrap = document.getElementById('board-wrap');
  const box = document.createElement('div');
  box.className = 'error-box';
  box.textContent = message;
  wrap.replaceChildren(box);
};

try {
  const game = new Game(document.getElementById('app'));
  window.kmines = game;          // handy for manual poking / the headless check
} catch (error) {
  showError(`KMines could not start: ${error.message}`);
  throw error;
}
