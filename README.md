# Introduction
Supplementary material for the book: **"A Practical Introduction to the Xilinx Zynq-7000 Adaptive SoC - Bare-Metal Fundamentals"**, written by Derek Murray.

Book Links:
Amazon.com: https://www.amazon.com/Practical-Introduction-Xilinx-Zynq-7000-Adaptive-ebook/dp/B09DZRYFRD
Amazon.co.uk: https://www.amazon.co.uk/Practical-Introduction-Xilinx-Zynq-7000-Adaptive-ebook/dp/B09DZRYFRD


## What you can find here
### Step-By-Step Guides
The most important content is found in the *step-by-step* directory; this contains comprehensive documents for developing the FPGA design and software projects in the textbook, along with the C code and FPGA constraint files. (The source files are found in the *files_for_import* directory.) The Digilent Zybo-Z7-20 is the main platform supported, although the Zybo-Z7-10 can also be used with no changes to C code or constraints. Files are also included for the Digilent/Avnet ZedBoard.

Two development flows are covered: 
1. **Vivado HLx plus Xilinx SDK** (Vivado-SDK Flow.pdf); For Xilnx software versions up to and including 2019.1.
1. **Vivado HLx plus Vitis IDE** (Vivado-Vitis IDE Flow.pdf); For Xilinx software versions >= 2020.1. (Note that Vivado HLx has been rebranded as Vivado ML Edition in June 2021.)

An optional guide for preparing the Zybo-Z7-20/10 platform is also provided ("Prepare Board Files.pdf").

**Update 26th September 2021: TCL Flow added:**
If the reader does not want to build the FPGA project in IP Integrator, they can instead use a TCL flow. This flow implements the design as far as the hardware hand-off stage; refer to ***TCL Flow.pdf*** for a comprehensive description.

### Host Applications
The host applications developed in Chapter 15 of the textbook are also provided in the *host_apps* directory. Currently, LabVIEW and Python examples are provided:
1. **LabVIEW**: The LabVIEW application is developed in LabVIEW Community Edition, and it is packaged in JKI VIPM format. A PDF doc is also provided with software download locations and package installation instructions.
2. **Python**: The python code is provided in the form of a Jupyter notebook (.ipynb format).

### Digilent Analog Discovery Logic Analyser Project Files
In the *digilent_waveforms_workspaces* directory, logic analyser set-up files are provided for Projects 4-10 in the textbook. The test connections match those found in the text.

### Tera Term configuration file
The authors Tera Term configuration file can be found in the *teraterm_config* directory.

### Preview Document
A preview document of the textbook can be found in the *book_preview_doc* directory. Note that while the preview is in color format, all book editions are currently in grayscale format.




