static inline void updateInput();
static inline void updateKeyboardKey(SDL_KeyboardEvent* key, bool press);
static inline bool getKey(const char key);
static inline void fpsMouse(bool fpsMouse);
static inline void updateMouseKey(bool right, bool press);
static inline bool getMouseKey(bool rightKey);

#define NUL (char)0
#define SOH (char)1
#define STX (char)2
#define ETX (char)3
#define EOT (char)4
#define ENQ (char)5
#define ACK (char)6
#define BEL (char)7
#define BS (char)8
#define HT (char)9
#define LF (char)10
#define VT (char)11
#define FF (char)12
#define CR (char)13
#define SO (char)14
#define SI (char)15
#define DLE (char)16
#define DC1 (char)17
#define DC2 (char)18
#define DC3 (char)19
#define DC4 (char)20
#define NAK (char)21
#define SYN (char)22
#define ETB (char)23
#define CAN (char)24
#define EM (char)25
#define SUB (char)26
#define ESC (char)27
#define FS (char)28
#define GS (char)29
#define RS (char)30
#define US (char)31
#define SP (char)32
