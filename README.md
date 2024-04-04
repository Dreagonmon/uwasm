# UWasm

the slowest wasm runtime! focus on flexible memory management.

Status: WIP

## Goal

* Can use software virtual memory.
* Can run module without loading the whole module into the memory.
* Less heap useage.
* Don't need to pass all the wasm spec test.

## Roadmap

* [ ] Parse wasm module
    * [ ] Parse sections
        * [*] Type section
        * [*] Import section
        * [ ] Function section
        * [ ] Table section
        * [ ] Memory section
        * [ ] Global section
        * [*] Export section
        * [ ] Start section
        * [ ] Element section
        * [ ] Code section
        * [ ] Data section
        * [ ] Data count section
    * [ ] Execution

## Links

Spec: [https://webassembly.github.io/spec/](https://webassembly.github.io/spec/)
