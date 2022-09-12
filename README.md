# barcode_scanner
Build the barcode reader in your device.

### Bit stream decoder
Describe：this sample for QR/Data Matrix decoding. You need to bit stream extract from 2D image first , and then do this job.
Input：Binary bit code (N by N matrix array) 
Output：Geting the decoding result (form char* 1D buffer)
Support：Windows (X64/86)、WinCE、linux (X86/64/ARM64) plafforms(You con build the codes using VScode / Visual Studio IDE)