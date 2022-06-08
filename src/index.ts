import ModuleFactory, { EdgesGeometryModule } from "../wasm/module";

export class EdgesGeometry {
  static async init(): Promise<EdgesGeometry> {
    return new EdgesGeometry(await ModuleFactory());
  }

  private constructor(private module: EdgesGeometryModule) {}

  /**
   * Generate a line list of edges from indexed or unindexed triangle vertices.
   * @param positions A vertex position array.
   * @param indices A triangle index array, or undefined if the positions are unindexed.
   * @param thresholdAngle An edge is only created if the angle (in degrees) between the face normals of the adjoining faces exceeds this value.
   * @returns A line list of edges. The length of the returned array will always be divisible by six since it holds <XYZ, XYZ> pairs.
   */
  createEdges(
    positions: Float32Array,
    indices: Uint32Array | undefined,
    thresholdAngle: number
  ): Float32Array {
    // const outputBuf = this.module._malloc(output.byteLength); // eslint-disable-line no-underscore-dangle

    const positionsBuf = this.module._malloc(positions.byteLength); // eslint-disable-line no-underscore-dangle
    this.module.HEAPF32.subarray(
      positionsBuf >> 2,
      (positionsBuf >> 2) + positions.length
    ).set(positions);

    const indicesBuf =
      indices != undefined ? this.module._malloc(indices.byteLength) : 0; // eslint-disable-line no-underscore-dangle
    if (indices != undefined) {
      this.module.HEAPU32.subarray(
        indicesBuf >> 2,
        (indicesBuf >> 2) + indices.length
      ).set(indices);
    }

    try {
      return this.module.createEdges(
        positionsBuf,
        positions.length,
        indicesBuf,
        indices?.length ?? 0,
        thresholdAngle
      );
    } finally {
      this.module._free(positionsBuf); // eslint-disable-line no-underscore-dangle
      if (indicesBuf !== 0) {
        this.module._free(indicesBuf); // eslint-disable-line no-underscore-dangle
      }
    }
  }
}
