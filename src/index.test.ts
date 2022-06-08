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

describe("wasm-edgesgeometry", () => {
  it("find edges", async () => {
    const edgesGeometry = await EdgesGeometry.init();
    const edges = edgesGeometry.createEdges(CUBE, undefined, 40);
    expect(edges).toHaveLength(12 * 6); // 12 edges, 24 vertices, 72 floats

    // Do the same with THREE.EdgesGeometry
    const geometry = new THREE.BufferGeometry();
    geometry.setAttribute("position", new THREE.BufferAttribute(CUBE, 3));
    const edgesGeometry2 = new THREE.EdgesGeometry(geometry, 40);
    const threeEdges = (
      edgesGeometry2.attributes.position as THREE.BufferAttribute
    ).array;
    expect(threeEdges).toHaveLength(12 * 6);

    for (let i = 0; i < edges.length; i++) {
      expect(edges[i]).toBeCloseTo(threeEdges[i]!);
    }
  });
});
