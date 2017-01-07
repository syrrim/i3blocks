# i3blocks

i3blocks is a highly flexible **status line** for the [i3](http://i3wm.org) 
window manager. It handles *clicks*, *signals* and *language-agnostic* user 
*scripts*. 

The content of each *block* (e.g. time, battery status, network state, ...) is 
the output of a *command* provided by the user. Blocks are updated on *click*, 
at a given *interval* of time or on a given *signal*, also specified by the 
user.

This is a fork of vivien/i3blocks, which aims to provide various enhancements
and alterations.

##Changes

###Scripts

Various scripts are either created or altered to better suit my usage. 

####Brightness

Brightness tracks the brightness of the screen, and allows modifications by
scrolling while hovering over the block. Because changing brightness requires
root, I opted to use setuid. Those of you familiar with setuid will know it is
not available from a script. So, I wrote a c program to provide the core
functionality, then wrote a script wrapper that calls it. 


####Music

I use MPD to manage the playing of music on my system. mpc is the canonical cli
for managing MPD. mpc's `--format` option doesn't support the current time, so I
was forced* to write a program in c to get the time from libmpdclient. music
then multiplexes together output from mpc and mpdstate to allow for easier
modification and control.

*Someone on Reddit posted the code, so I just copied it from there. It was
easier.


####Bandwidth

I wrote two versions of bandwidth. The first deduplicates upload and download
code, and forces you to create two instances in your config. The second one
takes the average bandwidth usage from the last n calls, where n = 10. This
second script is called bw.

####Battery

For battery, I basically just fucked around with colors and formatting. I think
this is fairly basic, and plan on adding a system to i3blocks to make it easier. 

####NM

I use network-manager-applet for wifi, so I decided to forgoe the included wifi
script, and use a wrapper around NM instead. 

####Volume

I couldn't make heads or tails or volume, so I renamed it volume-pre, and
rewrote volume as a wrapper around it. It's functionality is, much like
bandwidth, just to provide formatting. Interestingly, the formatting is solely
arround the value of the percentage. The percentage isn't meaningful to mainline
i3bar, but my fork allows it to be treated specially through the fill parameter.
I might at some point add first class support for fill, and formatting based on
its value. 

###syrrim/i3bar

My fork of i3bar support several options that require alterations to i3blocks'
header files. There is also some responsibility that has been shifted from the
bar to the statusline, that must be accounted for. 


###Configuration

This fork also maintains a configuration bringing together the two features
described above. 

- - -

Here is an example of status line, showing the time updated every 5 seconds, 
the volume updated only when i3blocks receives a SIGRTMIN+1, and click events.

```` ini
#default value, will be set for everything
color=#D2E7FF

[volume]
label=Volume:
command=amixer get Master | grep -E -o '[0-9]{1,3}?%' | head -1
interval=once
signal=1
# use 'pkill -RTMIN+1 i3blocks' after changing the volume

width=66
above=true
align=right

background=#005C87
fill_color=#00364F

[time]
command=date '+%D %T'
interval=1
const_width=a
width=0
align=center

background=#020103

[clickme]
label=
command=echo button=$BLOCK_BUTTON x=$BLOCK_X y=$BLOCK_Y
interval=1
width=66
align=left

background=#005C87
fill_color=#00364F
````

This example looks like so:

![](example-1.png)

You can use your own scripts, or the 
[ones](https://github.com/vivien/i3blocks/tree/master/scripts) provided with 
i3blocks. Feel free to contribute and improve them!

The default config will look like this:

![example](screenie.png)

The scripts provided by default may use external tools:

  * `mpstat` (often provided by the *sysstat* package) used by `cpu_usage`.

  * `acpi` (often provided by a package of the same name) used by `battery`.

  * `mpc && libmpdclient` (go search it on your package manager) used by `music`
    (width uses) `mpdstate` repectively

The user contributed scripts may also use external tools:

  * `playerctl` (available [here](https://github.com/acrisci/playerctl)) used by `mediaplayer`.
  * `sensors` (often provided by the *lm-sensors* package) used by `temperature`.

## Documentation

For more information about how it works, please refer to the 
[**manpage**](http://vivien.github.io/i3blocks).

You can also take a look at the
[i3bar protocol](http://i3wm.org/docs/i3bar-protocol.html) to see what 
possibilities it offers you.

Take a look at the [wiki](https://github.com/vivien/i3blocks/wiki) for examples 
of blocks and screenshots. If you want to share your ideas and status line, 
feel free to edit it!

## Installation

You may install i3blocks from source:

    $ git clone git://github.com/vivien/i3blocks
    $ cd i3blocks
    $ make clean debug # or make clean all
    # make install

Note: the generation of the manpage requires the `pandoc` utility, packaged in
common distributions as `pandoc`.

### Usage

  * Set your `status_command` in a bar block of your i3 config file:

        bar {
          status_command i3blocks
        }

  * For customization, copy the default i3blocks.conf into ~/.i3blocks.conf
    (e.g. `cp /etc/i3blocks.conf ~/.i3blocks.conf`)
  * Restart i3 (e.g. `i3-msg restart`)

## Copying

i3blocks is Copyright (C) 2014 Vivien Didelot<br />
See the file COPYING for information of licensing and distribution.
