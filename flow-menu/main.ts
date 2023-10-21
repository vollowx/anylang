import { FlowMenu } from "./flow-menu.js";

type Languages = "en-US" | "en-UK" | "zh-CN";
type Directions = "vertical" | "horizontal";

const archive = {
  delete: (index: number) => {},
  start: (index: number) => {},
};

const language = {
  set: (value: Languages) => {},
};

const direction = {
  set: (value: Directions) => {},
};

const pages = {
  main: {
    header: "flow menu",
    items: [
      {
        label: "new game",
        callback: startNewGame,
      },
      {
        label: "archives",
        callback: () => navigateTo("archives"),
      },
      {
        label: "settings",
        callback: () => navigateTo("settings"),
      },
    ],
  },
  archives: {
    header: "archives",
    items: [
      {
        label: "back",
        callback: () => navigateTo("main"),
      },
      {
        label: "archive 1",
        callback: () => navigateTo("archive1"),
      },
      {
        label: "archive 2",
        callback: () => navigateTo("archive2"),
      },
      {
        label: "archive 3",
        callback: () => navigateTo("archive3"),
      },
      {
        label: "archive 4",
        callback: () => navigateTo("archive4"),
      },
      {
        label: "archive 5",
        callback: () => navigateTo("archive5"),
      },
    ],
  },
  archive1: {
    header: "archive 1",
    items: [
      {
        label: "back",
        callback: () => navigateTo("archives"),
      },
      {
        label: "delete",
        callback: () => archive.delete(1),
      },
      {
        label: "start",
        callback: () => archive.start(1),
      },
    ],
  },
  archive2: {
    header: "archive 2",
    items: [
      {
        label: "back",
        callback: () => navigateTo("archives"),
      },
      {
        label: "delete",
        callback: () => archive.delete(2),
      },
      {
        label: "start",
        callback: () => archive.start(2),
      },
    ],
  },
  settings: {
    header: "settings",
    items: [
      {
        label: "back",
        callback: () => navigateTo("main"),
      },
      {
        label: "language",
        callback: () => navigateTo("settings-language"),
      },
      {
        label: "direction",
        callback: () => navigateTo("settings-direction"),
      },
    ],
  },
  "settings-language": {
    header: "language",
    items: [
      {
        label: "back",
        callback: () => navigateTo("main"),
      },
      {
        label: "English - The United States",
        callback: () => language.set("en-US"),
      },
      {
        label: "English - The United Kingdom",
        callback: () => language.set("en-UK"),
      },
      {
        label: "简体中文 - 中国大陆",
        callback: () => language.set("zh-CN"),
      },
    ],
  },
  "settings-direction": {
    header: "direction",
    items: [
      {
        label: "back",
        callback: () => navigateTo("main"),
      },
      {
        label: "vertical",
        callback: () => direction.set("vertical"),
      },
      {
        label: "horizontal",
        callback: () => direction.set("horizontal"),
      },
    ],
  },
};

const container = document.querySelector(".container") as HTMLElement;
const header = container!.querySelector(".header")!;
const items = container!.querySelector(".items")!;
const controller = new FlowMenu(container);

function navigateTo(where: string) {
  const page = pages[where];
  items.innerHTML = "";
  header.textContent = page.header;
  for (let i = 0; i < page.items.length; i++) {
    const item = document.createElement("button");
    item.textContent = page.items[i].label;
    item.addEventListener("click", page.items[i].callback);
    items.appendChild(item);
  }
  controller.init();
}

function startNewGame() {
  console.log("new game starting");
}

navigateTo("main");
