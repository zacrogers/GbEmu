
some_bytes = b'\x3c'*1000 # INC A
some_bytes = b'\x03'*1000 # INC BC
# some_bytes = b'\x04'*1000 # INC B

# Open file in binary write mode
# binary_file = open("INC_A.gb", "wb")
binary_file = open("INC_BC.gb", "wb")

# Write bytes to file
binary_file.write(some_bytes)

# Close file
binary_file.close()