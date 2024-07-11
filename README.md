# UWasm

the slowest wasm runtime! focus on flexible memory management.

Status: WIP

## Goal

* Can use software virtual memory.
* Can run module without loading the whole module into the memory.
* Less heap useage.
* Don't need to pass all the wasm spec test. Only minimal feature are there.

## Roadmap

* [ ] Parse wasm module
    * [ ] Parse sections
        * [x] Type section
        * [ ] Import section
            * [x] Import Function
            * [ ] Import Memory
        * [x] Function section
        * [ ] Table section
        * [ ] Memory section
        * [ ] Global section
        * [x] Export section
        * [ ] Start section
        * [ ] Element section
        * [ ] Code section
        * [ ] Data section
        * [ ] Data count section
    * [ ] Execution

## Links

Spec: [https://webassembly.github.io/spec/](https://webassembly.github.io/spec/)
