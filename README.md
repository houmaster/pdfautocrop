# pdfautocrop
A command line tool for automatically cropping the margins of PDF files.

    Usage: pdfautocrop [-options] [input]
      -i,  --input <file>      input PDF filename.
      -o,  --output <file>     output PDF filename.
      -ch, --crop-header [pt]  try to crop page headers.
      -cf, --crop-footer [pt]  try to crop page footers.
      -co, --crop-outlier      crop pages with larger than average extent.
      -m,  --margin <pt>       margin to add to each cropped page (default: 5).
          also available: margin-left, -right, -top, -bottom, -inner, -outer
      -r,  --resolution <dpi>  resolution of internal rendering (default: 96).
      -h,  --help              print this help.
