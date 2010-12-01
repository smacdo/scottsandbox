enum CompareFunction
{
    COMPARE_ALWAYS_FAIL,
    COMPARE_ALWAYS_PASS,
    COMPARE_LESS,
    COMPARE_LESS_EQUAL,
    COMPARE_EQUAL,
    COMPARE_NOT_EQUAL,
    COMPARE_GREATER_EQUAL,
    COMPARE_GREATER
};

enum CullingMode
{
    CULL_NONE = 1,
    CULL_CLOCKWISE,
    CULL_COUNTERCLOCKWISE
};

enum Waveform
{
    WF_SINE,
    WF_TRIANGLE,
    WF_SQUARE,
    WF_SAWTOOTH,
    WF_INVERSE_SAWTOOTH,
    // pulse width modulation
    WF_PWM
};

enum PolygonMode
{
    PM_POINTS,
    PM_WIREFRAME,
    PM_SOLID
};

