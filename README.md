Compact Tensile Tester for Carbon Nanotube Wires

This repository contains the mechanical design, Arduino firmware, and MATLAB analysis scripts developed for a compact tensile tester used to characterise fragile filamentary materials such as carbon nanotube (CNT) wires.
The device combines a stepper-motor actuator, miniature load cell, and linear potentiometer under Arduino control, achieving sub-micron displacement precision and milli-Newton force resolution.

Repository contents

/Design/ – STEP files for the mechanical frame, grips, and mounting components.

/Arduino/ – FINAL_NEW.ino firmware for actuator control, force and displacement acquisition, and serial data output.

/MATLAB/ –

loadPlot.m – Loads Arduino output and plots the load–displacement curve, applying smoothing and zero correction.

tensilePlotEng.m – Converts load–displacement data to engineering stress–strain, applies compliance correction, and calculates Young’s modulus (YM), ultimate tensile strength (UTS), and strain at break.

Getting started

Assemble the tester using the STEP files.

Upload FINAL_NEW.ino to an Arduino (tested with Arduino Uno).

Run a tensile test and save the serial output as .txt or .csv.

Process the data in MATLAB:

DATA = readtable('test_sample.csv');
loadPlot(DATA);                   % Visualise load–displacement
[YM, UTS, Break] = tensilePlotEng(DATA);  % Compute and plot stress–strain


Both MATLAB scripts automatically smooth, zero, and correct the data before plotting. During execution, tensilePlotEng.m prompts for lower and upper stress bounds to fit the elastic region and extract Young’s modulus.

Requirements

Hardware: Arduino Uno (or equivalent), 2 kg S-beam load cell with amplifier, linear potentiometer, Nema 11 stepper motor.

Software: Arduino IDE, MATLAB R2021a or later.
