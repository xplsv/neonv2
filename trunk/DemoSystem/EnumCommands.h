//---------------------------------------------------------------------------//
// File: EnumCommand.h
//
// Aqui se define la lista de comandos y su String asociado. Es un poco
// mas tedioso que pasar directamente el string, pero tambien mas rapido,
// y tampoco se añaden tantos comandos como para que esto sea coñazo (o eso espero),
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Effect commands
//---------------------------------------------------------------------------//
// Layer
ITEM(COMM_EFFECT, EC_RESET,                "Reset")
ITEM(COMM_EFFECT, EC_ENABLE_FX,            "Enable")
ITEM(COMM_EFFECT, EC_DISABLE_FX,           "Disable")
ITEM(COMM_EFFECT, EC_SET_FX_RENDER_TARGET, "SetFXRenderTarget")
ITEM(COMM_EFFECT, EC_SET_FX_ALPHA,         "SetFXAlpha")

// Common commands
ITEM(COMM_EFFECT, EC_SET_SPEED,            "SetSpeed")
ITEM(COMM_EFFECT, EC_SET_TIME,             "SetTime")
ITEM(COMM_EFFECT, EC_SET_BLEND_MODE,       "SetBlendMode")
ITEM(COMM_EFFECT, EC_MODULATE,             "Modulate")
ITEM(COMM_EFFECT, EC_ALPHA_FADE_IN,        "AlphaFadeIn")
ITEM(COMM_EFFECT, EC_ALPHA_FADE_OUT,       "AlphaFadeOut")

// FXImage commands
ITEM(COMM_EFFECT, EC_ENABLE_FFT_XFORM,     "EnableFFTXForm")


//---------------------------------------------------------------------------//
// Filter commands
//---------------------------------------------------------------------------//
// Common
ITEM(COMM_FILTER, FC_SETMODE,              "SetMode")
ITEM(COMM_FILTER, FC_RESET,                "Reset")
ITEM(COMM_FILTER, FC_FADEOUT,              "FadeOut")
ITEM(COMM_FILTER, FC_FADEIN,               "FadeIn")
ITEM(COMM_FILTER, FC_START,                "Start")
ITEM(COMM_FILTER, FC_PAUSE,                "Pause")
ITEM(COMM_FILTER, FC_SETCOLOR,             "SetColor")

// FilterBlur, FilterPixelate, FilterGlow
ITEM(COMM_FILTER, FC_INTENS,               "SetIntens")
ITEM(COMM_FILTER, FC_FXFADEOUT,            "FXFadeOut")
ITEM(COMM_FILTER, FC_FXFADEIN,             "FXFadeIn")
