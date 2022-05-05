# test for vias that are not centered and use of multi cut tech via
source "helpers.tcl"

read_lef asap7_vias/asap7_tech_1x_offcenter_via.lef
read_lef asap7_vias/asap7sc7p5t_27_R_1x.lef
read_def asap7_vias/floorplan.def

add_global_connection -net VDD -pin_pattern {^VDD$} -power
add_global_connection -net VSS -pin_pattern {^VSS$} -ground

set_voltage_domain -power VDD -ground VSS

define_pdn_grid -name "Core"
add_pdn_stripe -followpins -layer M1 -width 0.072
add_pdn_stripe -followpins -layer M2 -width 0.090

add_pdn_stripe -layer M3 -width 0.936 -spacing 1.512 -pitch 8.00 -offset 1.000

add_pdn_connect -layers {M1 M2}
add_pdn_connect -layers {M2 M3} -fixed_vias "VIA23_OFFCENTER"

pdngen

set def_file [make_result_file asap7_offcenter_via.def]
write_def $def_file
diff_files asap7_offcenter_via.defok $def_file
