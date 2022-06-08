import * as b from "benny";
import * as THREE from "three";

import { EdgesGeometry } from ".";

// prettier-ignore
const CUBE = new Float32Array([
  -1.0,-1.0,-1.0,
  -1.0,-1.0, 1.0,
  -1.0, 1.0, 1.0,

   1.0, 1.0,-1.0,
  -1.0,-1.0,-1.0,
  -1.0, 1.0,-1.0,

   1.0,-1.0, 1.0,
  -1.0,-1.0,-1.0,
   1.0,-1.0,-1.0,

   1.0, 1.0,-1.0,
   1.0,-1.0,-1.0,
  -1.0,-1.0,-1.0,

  -1.0,-1.0,-1.0,
  -1.0, 1.0, 1.0,
  -1.0, 1.0,-1.0,

   1.0,-1.0, 1.0,
  -1.0,-1.0, 1.0,
  -1.0,-1.0,-1.0,

  -1.0, 1.0, 1.0,
  -1.0,-1.0, 1.0,
   1.0,-1.0, 1.0,

   1.0, 1.0, 1.0,
   1.0,-1.0,-1.0,
   1.0, 1.0,-1.0,

   1.0,-1.0,-1.0,
   1.0, 1.0, 1.0,
   1.0,-1.0, 1.0,

   1.0, 1.0, 1.0,
   1.0, 1.0,-1.0,
  -1.0, 1.0,-1.0,

   1.0, 1.0, 1.0,
  -1.0, 1.0,-1.0,
  -1.0, 1.0, 1.0,

   1.0, 1.0, 1.0,
  -1.0, 1.0, 1.0,
   1.0,-1.0, 1.0
]);

const MANY = 5_000;
const MANY_CUBES = new Float32Array(CUBE.length * MANY);
for (let i = 0; i < MANY; i++) {
  const translation = [
    (Math.random() - 0.5) * 100,
    (Math.random() - 0.5) * 100,
    (Math.random() - 0.5) * 100,
  ];
  for (let j = 0; j < CUBE.length; j++) {
    MANY_CUBES[i * CUBE.length + j] = CUBE[j]! + translation[j % 3]!;
  }
}

void b.suite(
  "Cube",

  b.add("THREE.EdgesGeometry", () => {
    const geometry = new THREE.BufferGeometry();
    geometry.setAttribute("position", new THREE.BufferAttribute(CUBE, 3));

    return () => {
      new THREE.EdgesGeometry(geometry, 40);
    };
  }),

  b.add("WASM.EdgesGeometry", async () => {
    const edgesGeometry = await EdgesGeometry.init();

    return () => {
      edgesGeometry.createEdges(CUBE, undefined, 40);
    };
  }),

  b.cycle(),
  b.complete()
);

void b.suite(
  "Many Cubes",

  b.add("THREE.EdgesGeometry", () => {
    const geometry = new THREE.BufferGeometry();
    geometry.setAttribute("position", new THREE.BufferAttribute(MANY_CUBES, 3));

    return () => {
      new THREE.EdgesGeometry(geometry, 40);
    };
  }),

  b.add("WASM.EdgesGeometry", async () => {
    const edgesGeometry = await EdgesGeometry.init();

    return () => {
      edgesGeometry.createEdges(MANY_CUBES, undefined, 40);
    };
  }),

  b.cycle(),
  b.complete()
);
