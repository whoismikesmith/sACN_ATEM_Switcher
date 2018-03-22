# sACN_ATEM_Switcher
Control ATEM Switchers via sACN / DMX using an Arduino w/ Ethernet Sheild

The script monitors incoming sACN (DMX) data.
  Channel 1 - Select Aux Source Input 
  -scrolls through aux inputs 1-2 as you sweep through 0-100% of dmx channel level
  Channel 2 - Select Preview Video Input
  -scrolls through inputs 1-8 as you sweep through 0-100% of dmx channel level
  Channel 3 - Select Program Video Input
  -scrolls through inputs 1-8 as you sweep through 0-100% of dmx channel level
  Channel 10 - Cut
  -makes cut if dmx channel level rises above 50%
  
  
Based on work from skaarhoj.com under the CC-by-sa license
Read more at http://www.skaarhoj.com/about/licenses/#0mCuP9t1w8roJmCh.99

and

E1.31 Receiver and pixel controller by Andrew Huxtable (andrew@hux.net.au)
