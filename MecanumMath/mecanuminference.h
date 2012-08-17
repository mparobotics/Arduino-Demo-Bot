/*Mecanum Inference
Contains functions that allow inferences about a mecanum drive.
Instead of joystick -> wheel voltage multipliers, this header
does wheel voltage multipliers -> actual speed, turn rate, heading.
This could potentially be used for motion planning, simulation,
diagnostics [warn when predicted acceleration gets out of sync with
the accelerometer], etc. etc. Also theoretically possible to create
an application where drivers can train to use mecanum drives
and programmers can test different methods of controlling mecanum
drives without access to an actual mecanum drive. 

Contributors:
Alex Grabanski */
