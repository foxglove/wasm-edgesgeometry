// eslint-disable-next-line filenames/match-exported
export type EdgesGeometryModule = EmscriptenModule & {
  createEdges: (
    positionsPtr: number,
    positionsLen: number,
    indicesPtr: number,
    indicesLen: number,
    thresholdAngle: number
  ) => Float32Array;
};

const ModuleFactory: EmscriptenModuleFactory<EdgesGeometryModule>;
export default ModuleFactory;
