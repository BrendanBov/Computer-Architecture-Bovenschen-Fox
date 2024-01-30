onbreak {resume}

# create library
if [file exists work] {
    vdel -all
}
vlib work

# compile source files
vlog regfile.sv regfile_tb.sv

# start and run simulation
vsim -voptargs=+acc work.stimulus 

view list
view wave

-- display input and output signals as hexidecimal values
# Diplays All Signals recursively
#add wave -hex -r /stimulus/*

add wave -noupdate -divider -height 32 "Inputs"
add wave -bin /stimulus/clock
add wave -bin /stimulus/we3
add wave -hex /stimulus/ra1
add wave -hex /stimulus/ra2
add wave -hex /stimulus/wa3
add wave -hex /stimulus/wd3

add wave -noupdate -divider -height 32 "Outputs"
add wave -hex /stimulus/rd1
add wave -hex /stimulus/rd2

add wave -noupdate -divider -height 32 "Expected Outputs"
add wave -hex /stimulus/rd1exp
add wave -hex /stimulus/rd2exp
add wave -hex /stimulus/errors

add wave -noupdate -divider -height 32 "Registers"
add wave -hex /stimulus/dut/rf

-- Set Wave Output Items 
TreeUpdate [SetDefaultTree]
WaveRestoreZoom {0 ps} {75 ns}
configure wave -namecolwidth 150
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2

-- Run the Simulation
run 5000ns


