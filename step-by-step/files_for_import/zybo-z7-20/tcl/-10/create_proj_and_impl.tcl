#================================================================================#
# This script is compatible with the the Digilent Zybo-Z7-10. 
# The Digilent board files ("zybo-z7-10") must be in the relevant Vivado directory
# (i.e. C:\Xilinx\Vivado\<ver>\data\boards\board_files\zybo-z7-10).
# 
# The file is tested and working on versions 2017.4, 2018.3, 2019.1, 2020.2.
# In Vivado 2021.1 it will create the project, but the source files cannot be
# opened unless Vivado is restarted. However, it is still possible to export 
# the design and run Vitis to build the software projects (without restarting
# Vivado).(A fully working version will be added for 2021.1 if a fix is found.)
#
#================================================================================#

# Set some project variables
set proj_name hw_proj1
set proj_dir ./$proj_name
set bd_name hw_proj1

# Zybo-Z7-20 constraints also compatible with Zybo-Z7-10:
set constr_file F:/Documents/GitHub/book1-zynq-intro/step-by-step/files_for_import/zybo-z7-20/constraints/main_constraints.xdc

set part_name xc7z010clg400-1 
set board_part_name digilentinc.com:zybo-z7-10:part0:1.0


# ==== Create the project ==== #
create_project -force $proj_name $proj_dir -part $part_name


# ==== Set the board to zybo-z7-20 ==== #
set_property board_part $board_part_name [current_project]




#===========================================================#
# ==== Design Entry using IP Integrator ====
#===========================================================#
# *** START: Design Entry using IP Integrator ***

# ==== Create Block Design ==== #
create_bd_design $bd_name

#===============================#
# ==== ZYNQ PS block ====
#===============================#
# Create ZYNQ PS block
create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_0

# Design automation for Zynq PS
apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" apply_board_preset "1" Master "Disable" Slave "Disable" }  [get_bd_cells processing_system7_0]

# Configure Zynq PS
set_property -dict [list CONFIG.PCW_UIPARAM_DDR_DQS_TO_CLK_DELAY_0 {0} CONFIG.PCW_UIPARAM_DDR_DQS_TO_CLK_DELAY_1 {0} CONFIG.PCW_UIPARAM_DDR_DQS_TO_CLK_DELAY_2 {0} CONFIG.PCW_UIPARAM_DDR_DQS_TO_CLK_DELAY_3 {0} CONFIG.PCW_USE_FABRIC_INTERRUPT {1} CONFIG.PCW_IRQ_F2P_INTR {1} CONFIG.PCW_QSPI_GRP_SINGLE_SS_ENABLE {1} CONFIG.PCW_TTC0_PERIPHERAL_ENABLE {1} CONFIG.PCW_TTC1_PERIPHERAL_ENABLE {1} CONFIG.PCW_MIO_50_PULLUP {disabled} CONFIG.PCW_MIO_51_PULLUP {disabled}] [get_bd_cells processing_system7_0]


# ==== Create TTC pins and re-name ==== #
make_bd_pins_external  [get_bd_pins processing_system7_0/TTC0_WAVE0_OUT]
make_bd_pins_external  [get_bd_pins processing_system7_0/TTC0_WAVE1_OUT]
make_bd_pins_external  [get_bd_pins processing_system7_0/TTC0_WAVE2_OUT]
make_bd_pins_external  [get_bd_pins processing_system7_0/TTC1_WAVE0_OUT]
make_bd_pins_external  [get_bd_pins processing_system7_0/TTC1_WAVE1_OUT]
make_bd_pins_external  [get_bd_pins processing_system7_0/TTC1_WAVE2_OUT]

set_property name TTC0_WAVE0_OUT [get_bd_ports TTC0_WAVE0_OUT_0]
set_property name TTC0_WAVE1_OUT [get_bd_ports TTC0_WAVE1_OUT_0]
set_property name TTC0_WAVE2_OUT [get_bd_ports TTC0_WAVE2_OUT_0]
set_property name TTC1_WAVE0_OUT [get_bd_ports TTC1_WAVE0_OUT_0]
set_property name TTC1_WAVE1_OUT [get_bd_ports TTC1_WAVE1_OUT_0]
set_property name TTC1_WAVE2_OUT [get_bd_ports TTC1_WAVE2_OUT_0]

# Save
save_bd_design



#================================#
# ==== AXI GPIO block ====
#================================#

# Create GPIO block and configure:
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_gpio:2.0 axi_gpio_0
set_property -dict [list CONFIG.C_GPIO_WIDTH {8} CONFIG.C_GPIO2_WIDTH {12} CONFIG.C_IS_DUAL {1} CONFIG.C_ALL_INPUTS_2 {1} CONFIG.C_ALL_OUTPUTS {1}] [get_bd_cells axi_gpio_0]

# Create GPIO pins (and re-name)
make_bd_pins_external  [get_bd_pins axi_gpio_0/gpio_io_o]
make_bd_pins_external  [get_bd_pins axi_gpio_0/gpio2_io_i]
# Re-name
set_property name gpio0_out [get_bd_ports gpio_io_o_0]
set_property name gpio0_in [get_bd_ports gpio2_io_i_0]

# Design automation for AXI GPIO
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/processing_system7_0/M_AXI_GP0" intc_ip "New AXI Interconnect" Clk_xbar "Auto" Clk_master "Auto" Clk_slave "Auto" }  [get_bd_intf_pins axi_gpio_0/S_AXI]

# Save
save_bd_design



#===============================================#
# ==== AXI QUAD_SPI block ====
#===============================================#
# Create QSPI block and configure:
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_quad_spi:3.2 axi_quad_spi_0
set_property -dict [list CONFIG.C_USE_STARTUP {0} CONFIG.C_USE_STARTUP_INT {0}] [get_bd_cells axi_quad_spi_0]

# Create QSPI pins (and re-name)
make_bd_pins_external  [get_bd_pins axi_quad_spi_0/io0_o]
make_bd_pins_external  [get_bd_pins axi_quad_spi_0/io1_i]
make_bd_pins_external  [get_bd_pins axi_quad_spi_0/sck_o]
make_bd_pins_external  [get_bd_pins axi_quad_spi_0/ss_o]

# Re-name:
set_property name miso [get_bd_ports io1_i_0]
set_property name mosi [get_bd_ports io0_o_0]
set_property name sck [get_bd_ports sck_o_0]
set_property name cs_n [get_bd_ports ss_o_0]

# Design automation for AXI QSPI
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/processing_system7_0/M_AXI_GP0" intc_ip "/ps7_0_axi_periph" Clk_xbar "Auto" Clk_master "Auto" Clk_slave "Auto" }  [get_bd_intf_pins axi_quad_spi_0/AXI_LITE]

# QSPI ext_spi_clk is not connected; connect it to FCLK_CLK0
connect_bd_net [get_bd_pins axi_quad_spi_0/ext_spi_clk] [get_bd_pins processing_system7_0/FCLK_CLK0]

# Save
save_bd_design



#===============================================#
# ==== CONCAT block ====
#===============================================#

# Create CONCAT block for PmodACL interrupt signals.
# No need to configure; default configuration is two inputs,
# which is all we need in this case.
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_0

create_bd_port -dir I -type intr PMOD_ACL_INT1
create_bd_port -dir I -type intr PMOD_ACL_INT2

connect_bd_net [get_bd_ports PMOD_ACL_INT1] [get_bd_pins xlconcat_0/In0]
connect_bd_net [get_bd_ports PMOD_ACL_INT2] [get_bd_pins xlconcat_0/In1]
connect_bd_net [get_bd_pins processing_system7_0/IRQ_F2P] [get_bd_pins xlconcat_0/dout]

# Save
save_bd_design



#===============================================#
# ==== Block Diagram Tidy-up ====
#===============================================#

# Regenerate layout
regenerate_bd_layout -routing
regenerate_bd_layout

# Validate and save
validate_bd_design
save_bd_design

#===============================================#
# ==== GENERATE OUTPUT PRODUCTS ====
#===============================================#

generate_target all [get_files  $proj_dir/$proj_name.srcs/sources_1/bd/$bd_name/$bd_name.bd]

catch { config_ip_cache -export [get_ips -all hw_proj1_processing_system7_0_0] }
catch { config_ip_cache -export [get_ips -all hw_proj1_axi_gpio_0_0] }
catch { config_ip_cache -export [get_ips -all hw_proj1_rst_ps7_0_50M_0] }
catch { config_ip_cache -export [get_ips -all hw_proj1_axi_quad_spi_0_0] }
catch { config_ip_cache -export [get_ips -all hw_proj1_xlconcat_0_0] }
catch { config_ip_cache -export [get_ips -all hw_proj1_xbar_0] }
catch { config_ip_cache -export [get_ips -all hw_proj1_auto_pc_0] }

export_ip_user_files -of_objects [get_files $proj_dir/$proj_name.srcs/sources_1/bd/$bd_name/$bd_name.bd] -no_script -sync -force -quiet

create_ip_run [get_files -of_objects [get_fileset sources_1] $proj_dir/$proj_name.srcs/sources_1/bd/$bd_name/$bd_name.bd]

launch_runs -jobs 3 {hw_proj1_processing_system7_0_0_synth_1 hw_proj1_axi_gpio_0_0_synth_1 hw_proj1_rst_ps7_0_50M_0_synth_1 hw_proj1_axi_quad_spi_0_0_synth_1 hw_proj1_xlconcat_0_0_synth_1 hw_proj1_xbar_0_synth_1 hw_proj1_auto_pc_0_synth_1}


export_simulation -of_objects [get_files $proj_dir/$proj_name.srcs/sources_1/bd/$bd_name/$bd_name.bd] -directory $proj_dir/$proj_name.ip_user_files/sim_scripts -ip_user_files_dir $proj_dir/$proj_name.ip_user_files -ipstatic_source_dir $proj_dir/$proj_name.ip_user_files/ipstatic -lib_map_path [list {modelsim=$proj_dir/$proj_name.cache/compile_simlib/modelsim} {questa=$proj_dir/$proj_name.cache/compile_simlib/questa} {riviera=$proj_dir/$proj_name.cache/compile_simlib/riviera} {activehdl=$proj_dir/$proj_name.cache/compile_simlib/activehdl}] -use_ip_compiled_libs -force -quiet


#===============================================#
# ==== GENERATE WRAPPER FILE ====
#===============================================#

make_wrapper -files [get_files $proj_dir/$proj_name.srcs/sources_1/bd/$bd_name/$bd_name.bd] -top
add_files -norecurse $proj_dir/$proj_name.srcs/sources_1/bd/$bd_name/hdl/hw_proj1_wrapper.v

update_compile_order -fileset sources_1


#===============================================#
# ==== IMPORT CONSTRAINTS FILE ====
#===============================================#
# import_files -fileset constrs_1 
import_files -fileset constrs_1 $constr_file


#===============================================#
# RUN SYNTHESIS, IMPLEMENTATION, BIT-FILE:
#===============================================#
launch_runs impl_1 -to_step write_bitstream -jobs 3
wait_on_run impl_1


# *** END: IMPLEMENTATION PHASE ***
# ------------------------------------------------------#



#===============================================#
# ==== LAUNCH VIVADO ====
#===============================================#

start_gui



# *** END: Design Entry using IP Integrator ***
# ------------------------------------------------------#


