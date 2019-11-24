#========================================================#
# === TTC0/1 => PMOD JC ===
#========================================================#
set_property IOSTANDARD LVCMOS33 [get_ports TTC0_WAVE0_OUT]
set_property IOSTANDARD LVCMOS33 [get_ports TTC0_WAVE1_OUT]
set_property IOSTANDARD LVCMOS33 [get_ports TTC0_WAVE2_OUT]
set_property IOSTANDARD LVCMOS33 [get_ports TTC1_WAVE0_OUT]
set_property IOSTANDARD LVCMOS33 [get_ports TTC1_WAVE1_OUT]
set_property IOSTANDARD LVCMOS33 [get_ports TTC1_WAVE2_OUT]

# TTC0_WAVE0 => PIN 1 = V15
# TTC0_WAVE1 => PIN 2 = W15
# TTC0_WAVE2 => PIN 3 = T11
# TTC1_WAVE0 => PIN 7 = W14
# TTC1_WAVE1 => PIN 8 = Y14
# TTC1_WAVE2 => PIN 9 = T12
set_property PACKAGE_PIN V15 [get_ports TTC0_WAVE0_OUT]
set_property PACKAGE_PIN W15 [get_ports TTC0_WAVE1_OUT]
set_property PACKAGE_PIN T11 [get_ports TTC0_WAVE2_OUT]
# PIN 4 (T10) NOT USED
set_property PACKAGE_PIN W14 [get_ports TTC1_WAVE0_OUT]
set_property PACKAGE_PIN Y14 [get_ports TTC1_WAVE1_OUT]
set_property PACKAGE_PIN T12 [get_ports TTC1_WAVE2_OUT]
# PIN 10 (U12) NOT USED



#========================================================#
# === PmodACL => PMOD JD ===
#========================================================#
set_property IOSTANDARD LVCMOS33 [get_ports cs_n]
set_property IOSTANDARD LVCMOS33 [get_ports mosi]
set_property IOSTANDARD LVCMOS33 [get_ports miso]
set_property IOSTANDARD LVCMOS33 [get_ports sck]
set_property IOSTANDARD LVCMOS33 [get_ports PMOD_ACL_INT2]
set_property IOSTANDARD LVCMOS33 [get_ports PMOD_ACL_INT1]

# CSn => PMOD JD PIN 1 = T14
# SDI => PMOD JD PIN 2 = T15
# SDO => PMOD JD PIN 3 = P14
# SCLK => PMOD JD PIN 4 = R14
# PMOD_ACL_INT2 => PMOD JD PIN 7 = U14
# PMOD_ACL_INT1 => PMOD JD PIN 8 = U15
set_property PACKAGE_PIN T14 [get_ports cs_n]
set_property PACKAGE_PIN T15 [get_ports mosi]
set_property PACKAGE_PIN P14 [get_ports miso]
set_property PACKAGE_PIN R14 [get_ports sck]
set_property PACKAGE_PIN U14 [get_ports PMOD_ACL_INT2]
set_property PACKAGE_PIN U15 [get_ports PMOD_ACL_INT1]


#========================================================#
# === AXI GPIO0 ===
#========================================================#
set_property IOSTANDARD LVCMOS33 [get_ports gpio0_out*]
set_property IOSTANDARD LVCMOS33 [get_ports gpio0_in*]

#===============
# gpio0_out[7:0]
#===============
# Board LEDs: LD0 - LD3
set_property PACKAGE_PIN M14 [get_ports {gpio0_out[0]}]
set_property PACKAGE_PIN M15 [get_ports {gpio0_out[1]}]
set_property PACKAGE_PIN G14 [get_ports {gpio0_out[2]}]
set_property PACKAGE_PIN D18 [get_ports {gpio0_out[3]}]

# PMOD JE Pins 1-4
set_property PACKAGE_PIN V12 [get_ports {gpio0_out[4]}]
set_property PACKAGE_PIN W16 [get_ports {gpio0_out[5]}]
set_property PACKAGE_PIN J15 [get_ports {gpio0_out[6]}]
set_property PACKAGE_PIN H15 [get_ports {gpio0_out[7]}]

#===============
# gpio0_in[11:0]
#===============
# Board Push-buttons: BTN0 - BTN3
set_property PACKAGE_PIN K18 [get_ports {gpio0_in[0]}]
set_property PACKAGE_PIN P16 [get_ports {gpio0_in[1]}]
set_property PACKAGE_PIN K19 [get_ports {gpio0_in[2]}]
set_property PACKAGE_PIN Y16 [get_ports {gpio0_in[3]}]

# Board Switches: SW0 - SW3
set_property PACKAGE_PIN G15 [get_ports {gpio0_in[4]}]
set_property PACKAGE_PIN P15 [get_ports {gpio0_in[5]}]
set_property PACKAGE_PIN W13 [get_ports {gpio0_in[6]}]
set_property PACKAGE_PIN T16 [get_ports {gpio0_in[7]}]

# PMOD JE Pins 5-8
set_property PACKAGE_PIN V13 [get_ports {gpio0_in[8]}]
set_property PACKAGE_PIN U17 [get_ports {gpio0_in[9]}]
set_property PACKAGE_PIN T17 [get_ports {gpio0_in[10]}]
set_property PACKAGE_PIN Y17 [get_ports {gpio0_in[11]}]

