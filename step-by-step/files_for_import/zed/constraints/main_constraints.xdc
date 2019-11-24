#========================================================#
# === TTC0/1 => PMOD JB ===
#========================================================#
set_property IOSTANDARD LVCMOS33 [get_ports TTC0_WAVE0_OUT]
set_property IOSTANDARD LVCMOS33 [get_ports TTC0_WAVE1_OUT]
set_property IOSTANDARD LVCMOS33 [get_ports TTC0_WAVE2_OUT]
set_property IOSTANDARD LVCMOS33 [get_ports TTC1_WAVE0_OUT]
set_property IOSTANDARD LVCMOS33 [get_ports TTC1_WAVE1_OUT]
set_property IOSTANDARD LVCMOS33 [get_ports TTC1_WAVE2_OUT]

# TTC0_WAVE0 => PIN 1 = W12
# TTC0_WAVE1 => PIN 2 = W11
# TTC0_WAVE2 => PIN 3 = V10
# TTC1_WAVE0 => PIN 7 = V12
# TTC1_WAVE1 => PIN 8 = W10
# TTC1_WAVE2 => PIN 9 = V9
set_property PACKAGE_PIN W12 [get_ports TTC0_WAVE0_OUT]
set_property PACKAGE_PIN W11 [get_ports TTC0_WAVE1_OUT]
set_property PACKAGE_PIN V10 [get_ports TTC0_WAVE2_OUT]
# PIN 4 (W8) NOT USED
set_property PACKAGE_PIN V12 [get_ports TTC1_WAVE0_OUT]
set_property PACKAGE_PIN W10 [get_ports TTC1_WAVE1_OUT]
set_property PACKAGE_PIN V9 [get_ports TTC1_WAVE2_OUT]
# PIN 10 (V8) NOT USED




#========================================================#
# === AXI GPIO0 ===
#========================================================#
set_property IOSTANDARD LVCMOS33 [get_ports gpio0_out*]
set_property IOSTANDARD LVCMOS33 [get_ports gpio0_in*]

#===============
# gpio_out[7:0]
#===============
# Board LEDs: LD0 - LD7
set_property PACKAGE_PIN T22 [get_ports {gpio0_out[0]}]
set_property PACKAGE_PIN T21 [get_ports {gpio0_out[1]}]
set_property PACKAGE_PIN U22 [get_ports {gpio0_out[2]}]
set_property PACKAGE_PIN U21 [get_ports {gpio0_out[3]}]
# set_property PACKAGE_PIN V22 [get_ports {gpio0_out[4]}]
# set_property PACKAGE_PIN W22 [get_ports {gpio0_out[5]}]
# set_property PACKAGE_PIN U19 [get_ports {gpio0_out[6]}]
# set_property PACKAGE_PIN U14 [get_ports {gpio0_out[7]}]

# PMOD JC Pins 1-4
set_property PACKAGE_PIN AB7 [get_ports {gpio0_out[4]}]
set_property PACKAGE_PIN AB6 [get_ports {gpio0_out[5]}]
set_property PACKAGE_PIN Y4 [get_ports {gpio0_out[6]}]
set_property PACKAGE_PIN AA4 [get_ports {gpio0_out[7]}]


#===============
# gpio_in[11:0]
#===============
# Board Push-buttons: BTNU/BTNR/BTND/BTNL
set_property PACKAGE_PIN T18 [get_ports {gpio0_in[0]}]
set_property PACKAGE_PIN R18 [get_ports {gpio0_in[1]}]
set_property PACKAGE_PIN R16 [get_ports {gpio0_in[2]}]
set_property PACKAGE_PIN N15 [get_ports {gpio0_in[3]}]

# Board Switches: SW0 - SW3
set_property PACKAGE_PIN F22 [get_ports {gpio0_in[4]}]
set_property PACKAGE_PIN G22 [get_ports {gpio0_in[5]}]
set_property PACKAGE_PIN H22 [get_ports {gpio0_in[6]}]
set_property PACKAGE_PIN F21 [get_ports {gpio0_in[7]}]

# PMOD JC Pins 7-10
set_property PACKAGE_PIN R6 [get_ports {gpio0_in[8]}]
set_property PACKAGE_PIN T6 [get_ports {gpio0_in[9]}]
set_property PACKAGE_PIN T4 [get_ports {gpio0_in[10]}]
set_property PACKAGE_PIN U4 [get_ports {gpio0_in[11]}]


#========================================================#
# === PmodACL => PMOD JD ===
#========================================================#
set_property IOSTANDARD LVCMOS33 [get_ports cs_n]
set_property IOSTANDARD LVCMOS33 [get_ports mosi]
set_property IOSTANDARD LVCMOS33 [get_ports miso]
set_property IOSTANDARD LVCMOS33 [get_ports sck]
set_property IOSTANDARD LVCMOS33 [get_ports PMOD_ACL_INT2]
set_property IOSTANDARD LVCMOS33 [get_ports PMOD_ACL_INT1]

# cs_n => PMOD JD PIN 1 = W7
# mosi => PMOD JD PIN 2 = V7
# miso => PMOD JD PIN 3 = V4
# sclk => PMOD JD PIN 4 = V5
# PMOD_ACL_INT2 => PMOD JD PIN 7 = W5
# PMOD_ACL_INT1 => PMOD JD PIN 8 = W6
set_property PACKAGE_PIN V7 [get_ports cs_n]
set_property PACKAGE_PIN W7 [get_ports mosi]
set_property PACKAGE_PIN V5 [get_ports miso]
set_property PACKAGE_PIN V4 [get_ports sck]
set_property PACKAGE_PIN W6 [get_ports PMOD_ACL_INT2]
set_property PACKAGE_PIN W5 [get_ports PMOD_ACL_INT1]