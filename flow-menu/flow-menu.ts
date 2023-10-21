export class FlowMenu {
  el: HTMLElement;
  get items(): HTMLButtonElement[] {
    return [...this.el!.querySelectorAll("button")];
  }
  get indicator(): HTMLElement {
    return this.el!.querySelector(".indicator")!;
  }
  constructor(el: HTMLElement) {
    this.el = el;
    document.addEventListener("keydown", this.handleKeyDown.bind(this));
    document.addEventListener("keyup", this.handleKeyUp.bind(this));
    window.addEventListener("resize", this.handleResize.bind(this));
  }

  init() {
    this.items.forEach((item) => {
      item.setAttribute("tabindex", "-1");
    });

    this.focus(0);
  }

  get selected() {
    const index = this.items.indexOf(this.el!.querySelector("[tabindex='0']")!);
    if (index !== -1) return index;
    else return 0;
  }
  focus(index: number) {
    const rect = this.items[index].getBoundingClientRect();
    ["top", "left", "width", "height"].forEach(
      (property) => (this.indicator.style[property] = rect[property] + "px"),
    );
    this.items[this.selected].setAttribute("tabindex", "-1");
    this.items[index].setAttribute("tabindex", "0");
    this.items[index].focus();
  }
  handleKeyDown(e: KeyboardEvent) {
    const { key } = e;
    let targetI = 0;
    console.log(key);
    switch (key) {
      case "k":
      case "ArrowUp":
        e.preventDefault();
        targetI = this.selected - 1;
        targetI < 0 ? this.indicator.classList.add("up") : this.focus(targetI);
        break;

      case "j":
      case "ArrowDown":
        e.preventDefault();
        targetI = this.selected + 1;
        targetI > this.items.length - 1
          ? this.indicator.classList.add("down")
          : this.focus(targetI);
        break;

      case " ":
        this.indicator.classList.add("press");

      default:
        break;
    }
  }
  handleKeyUp(e: KeyboardEvent) {
    if (e.key === " ") {
      this.indicator.classList.remove("press");
    }
    this.indicator.classList.remove("up");
    this.indicator.classList.remove("down");
  }
  handleResize() {
    this.focus(this.selected);
  }
}
