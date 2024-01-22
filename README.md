# @foxglove/wasm-edgesgeometry

[![npm version](https://img.shields.io/npm/v/@foxglove/wasm-edgesgeometry)](https://www.npmjs.com/package/@foxglove/wasm-edgesgeometry)

THREE.js EdgesGeometry implementation in C++ compiled to WebAssembly

## Introduction

This package provides a WebAssembly build of https://threejs.org/docs/#api/en/geometries/EdgesGeometry, a helper method to generate a line list of edges from indexed or unindexed triangles. Run `yarn bench` to compare the performance of this library against the THREE.js implementation. It should be close to 10x faster for anything from a simple cube to >1MM triangles.

## Usage

```ts
import { EdgesGeometry } from "@foxglove/wasm-edgesgeometry";

const positions = new Float32Array([
  0.0, 0.0, 0.0,
  1.0, 0.0, 0.0,
  0.0, 1.0, 0.0,
  0.0, 0.0, 1.0,
]);
const indices = new Uint32Array([
  0, 1, 2,
  0, 2, 3,
]);

const edgesGeometry = await EdgesGeometry.init();

try {
  // Generate edges from positions and indices with a threshold angle of 40
  // degrees. If positions is unindexed, set indices to undefined
  const result = edgesGeometry.createEdges(positions, indices, 40);
  // The returned Float32Array is a view into WASM memory that will be
  // overwritten on the next call to createEdges. We create a copy here to avoid
  // the underlying memory being overwritten
  const lineListBuffer = result.slice();
  // This should print 30: 5 lines made up of <XYZ><XYZ>
  console.log(lineListBuffer.length);
} catch (error) {
  console.error(error);
}
```

## Stay in touch

Join our [Slack channel](https://foxglove.dev/slack) to ask questions, share feedback, and stay up to date on what our team is working on.
