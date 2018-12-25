make 

echo running halt 
./um unit_tests/halt.um > results/halt.txt

echo running halt-verbose 
./um unit_tests/halt-verbose.um > results/halt-verbose.txt

echo running emit_add_test
./um unit_tests/emit_add_test.um > results/emit_add_test.txt

echo running emit_conditional_move
./um unit_tests/emit_conditional_move.um > results/emit_conditional_move.txt 

echo running emit_bitwise_NAND
./um unit_tests/emit_bitwise_NAND.um > results/emit_bitwise_NAND.txt

echo running emit_mult_test
./um unit_tests/emit_mult_test.um > results/emit_mult_test.txt

echo running emit_div_test
./um unit_tests/emit_div_test.um > results/emit_div_test.txt

echo running emit_load_seg_test
./um unit_tests/emit_load_seg_test.um > results/emit_load_seg_test.txt

echo running emit_store_seg_test
./um unit_tests/emit_store_seg_test.um > results/emit_store_seg_test.txt

echo running emit_input_test
./um unit_tests/emit_input_test.um < input.txt > results/emit_input_test.txt

echo running emit_store_seg_large_test
./um unit_tests/emit_store_seg_large_test.um > results/emit_store_seg_large_test.txt

echo running emit_map_seg_test
./um unit_tests/emit_map_seg_test.um > results/emit_map_seg_test.txt

echo running emit_unmap_test
./um unit_tests/emit_unmap_test.um > results/emit_unmap_test.txt

echo running emit_load_program_test
./um unit_tests/emit_load_program_test.um > results/emit_load_program_test.txt

echo COMPLETE
