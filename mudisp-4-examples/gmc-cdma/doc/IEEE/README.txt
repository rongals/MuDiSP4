
August 13, 2002

This IEEEtran.cls is now the official LaTeX class for authors of the
Institute of Electrical and Electronics Engineers (IEEE) transactions
journals and conferences. IEEEtran.cls is mirrored within IEEE's site
as well.

The most recent version of IEEEtran.cls is version 1.6. IEEEtran.cls now
provides support for conferences and A4 paper. Users of previous versions
should no longer manually alter margins, paper sizes, page styles, etc.
Also, please note that \author is used slightly differently from that of
V1.5, especially for conferences. \pubid (if used) must now be placed
before \maketitle. \cite is now robust and can be used within captions
without a proceeding \protect.

As with previous versions, The itemize, enumerate, and description lists as
well as \appendix(ces) have enhanced controls and work slightly differently
than is done in traditional LaTeX.

Extensive formal documentation is now available. Enjoy! ;) The appendices
of the user manual contain important information on avoiding commonly
made mistakes, compatibility issues and installation instructions.


Best wishes for all your publication endeavors,

Michael Shell
mshell@ece.gatech.edu


***************** Subdirectories *****************

bibtex   - The IEEEtran BibTeX style package.

extras   - Supplementary material that may be of interest.

testflow - The testflow diagnostic suite for verifying PS/PDF output.

tools    - The IEEEtrantools.sty package which provides several commands
           from the IEEEtran.cls file for use with other LaTeX classes.


********************* Files **********************

README             - This file.

IEEEtran.cls       - The LaTeX class file.

IEEEtran_HOWTO.pdf - The user manual.

bare_conf.tex      - A "bare bones" starter file for conference papers.

bare_jrnl.tex      - A "bare bones" starter file for journal papers.




**********************************************************************
 Legal Notice:
 This code and advice is offered as-is without any warranty either
 expressed or implied; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE!
 User assumes all risk.
 In no event shall IEEE or any contributor to this code
 be liable for any damages or losses, including, but not limited to,
 incidental, consequential, or any other damages, resulting from the
 use or misuse of any information contained here.
 
 All statements made here are the opinions of their respective
 authors and are not necessarily endorsed by the IEEE.

 This code/text is distributed under the Perl Artistic License 
 ( http://language.perl.com/misc/Artistic.html ) 
 and may be freely used, distributed and modified.
 Retain all contribution notices and credits.
**********************************************************************


