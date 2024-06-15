# HTML Elements

## TODO

> Delete after implementation

### BlockAttr

```c
// Declaration
WEBCAPI void Address(char** buffer, Attribute** attributes, BlockContents contents);

// Definition
WEBCAPI void Address(char** buffer, Attribute** attributes, BlockContents contents)
{
    BlockAttr(buffer, "address", attributes, contents);
}
```

- `<area>`
- `<article>`
- `<aside>`
- `<audio>`
- `<canvas>`
- `<col>`
- `<colgroup>`
- `<data>`
- `<datalist>`
- `<details>`
- `<embed>`
- `<fieldset>`
- `<dialog>`
- `<dl>`
- `<figure>`
- `<footer>`
- `<form>` 
- `<iframe>`
- `<hgroup>`
- `<main>`
- `<map>`
- `<menu>`
- `<nav>`
- `<noscript>`
- `<object>`
- `<optgroup>`
- `<picture>`
- `<ruby>`
- `<search>`
- `<section>`
- `<select>`
- `<svg>`
- `<table>`
- `<tbody>`
- `<tfoot>`
- `<thead>`
- `<tr>`
- `<video>`
- `<template>`
- `<textarea>`

### InlineBlock

```c
// Declaration
WEBCAPI void Abbr(char** buffer, Attribute** attributes, Cstr abbr);

// Definition
WEBCAPI void Abbr(char** buffer, Attribute** attributes, Cstr abbr)
{
    InlineBlock(buffer, "abbr", attributes, abbr);
}
```

- `<b>`
- `<bdo>`
- `<bdi>`
- `<caption>`
- `<dd>`
- `<del>`
- `<dfn>`
- `<figcaption>`
- `<i>`
- `<dt>`
- `<em>`
- `<ins>`
- `<kbd>`
- `<label>`
- `<legend>`
- `<meter>`
- `<mark>`
- `<option>`
- `<output>`
- `<param>`
- `<pre>`
- `<progress>`
- `<q>`
- `<rp>`
- `<rt>`
- `<s>`
- `<samp>`
- `<small>`
- `<span>`
- `<strong>`
- `<sub>`
- `<summary>`
- `<time>`
- `<th>`
- `<u>`
- `<var>`
- `<wbr>`
- `<sup>`
- `<td>`

### Both

```c
// Declaration
WEBCAPI void Paragraph(char** buffer, Attribute** attributes, Cstr text);
WEBCAPI void ParagraphEx(char** buffer, Attribute** attributes, BlockContents contents);

// Definition as above
```

- `<button>`

### Single Tag

```c
#define Input(buffer, attr, ...) \
    Append(buffer, TagToString(MakeTag("input", attr, ##__VA_ARGS__)))
```

- `<source>`
- `<track>`



