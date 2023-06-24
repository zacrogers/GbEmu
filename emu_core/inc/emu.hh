// #pragma once

// #include "../inc/cpu.hh"
// #include "../inc/registers.hh"
// #include "../inc/bus.hh"
// #include "../inc/cart.hh"


// class Controls
// {
// public:
//     enum class Input
//     {
//         up, down, left, right,
//         a, b, start, select,
//         l1, l2, r1, r2, none
//     };

//     virtual bool init() = 0;
//     virtual void poll() = 0;

//     inline Input getLastPressed() { return lastPressed; }

// protected:
//     Input lastPressed = Input::none;
// };


// class Graphics
// {
// public:
//     virtual bool init() = 0;
//     virtual void draw() = 0;
// };


// class Emulator
// {
// public:
//     Emulator(Graphics *g, Controls *c): pGraphics(g), pControls(c) { }

//     virtual bool init()              = 0;
//     virtual bool loadRom(char *name) = 0;
//     virtual bool run()               = 0;

// protected:
//     Graphics          *pGraphics;
//     Controls          *pControls;

//     DMG01::Cart       cart    { };
//     DMG01::Registers  reg     { };
//     DMG01::Bus        bus     { &cart };
//     DMG01::Cpu        cpu     { &reg, &bus };

//     bool              running { false };
//     bool              paused  { false };
//     uint64_t          ticks   { 0 };
// };
