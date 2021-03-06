# MachineKit Stepper Driver

![stepper](/images/onmachine.jpg)

Uses TMC2660  
Uses ATSAMD51  
Uses AS5147D

Does networked control of stepper motors w/ up to 2.8A current. Plans for closed loop step-servo control.

See [circuit chatter](/circuit) and [programming chatter](/embedded).

## Commands

Position:
key: 129, units: int32_t, steps

Speed:
key: 130, units: uint32_t, steps/s

Block:
key: 131, entry speed, acceleration, cruise speed, exit speed, steps

# Todo

Redo doc as if it's a real project. Update board for AEAT6600 Encoder.

### Step Driver

I'm using the TMC2660 - this driver is 'fully integrated' as in it has current control circuitry, microstepping logic etc *as well as* a set of MOSFETS to drive the motor. I like this one because it has all of the modern step-driver bells and whistles, and has a counterpart - the TMC262 - that contains only the gate-driving circuitry, which will (later on) let me make a board for *really big* stepper motors, using external MOSFETS rated to however-many-amps-I-want.

### Microcontroller

This stepdriver is part of another project on networked machine control (I am calling this MachineKit, although I may have to change this as I am now realizing that machinekit.com is taken). My favourite new microcontroller for those endpoints is the ATSAMD51, so it's the stand-in. I would also consider the ATSAMD21 (these are both very well supported by 'the community') and the 51 is newer, faster, etc, at a price increase from $2 -> $4. But, in the interests of only dealing with one set of registers, bootloaders, packages, etc - I am just going to not think too hard and pick the ATSAMD51 for this bb, as well. Future firmware-writing self will thank me.

### To Encoder or not to Encoder?

[Mechaduino](http://tropical-labs.com/index.php/mechaduino) is a really excellent project that Sam and I are fond of. They use an AS5047 encoder to resolve more-steps-than-should-be-possible-with-a-stepper. In combining this with the fancy drive technology in the TMC chip, we can basically arrive at a low cost closed-loop motion control solution, so that's pretty rad. I'm familiar with this chip and use it in my brushless motor controller, so it's feeling quite likely I will implement it here as well. My one quam is the added complexity, etc. I could mount it right in dead-center of the board, use side-mount leds for indication and then have a nice clean backplate... (I am trying to avoid a two sided board). I had also thought I would perhaps have the possibility for an external encoder-only board... but, again, less complexity the better. Backplate mounter. OK.

Also [smartstepper](https://hackaday.io/project/18468-smart-stepper)

## Form Factor

A-la [Mechaduino](http://tropical-labs.com/index.php/mechaduino) and many other solutions, this beckons to be mounted on the arse-side of the motor it is responsible for... that way, it looks neat, and we can just glue a magnet on the back of the shaft to read position with our encoder. In the future, I really want to also control NEMA8 and NEMA11 motors (because they're cute as heck) but for the immediate future NEMA17s are OK and I can always use a laser-cut adapter to mount the same board onto a NEMA23. OK, again, simplicity wins, we go for the NEMA17 board, but spec drivers to NEMA23 size.

# Currently

Ready for MKXMods network integration.

![fabbed](https://gitlab.cba.mit.edu/jakeread/mkstepper/raw/master/images/routed.png)