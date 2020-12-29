%% This is file `ibook.cls'.
%% Copyright (C) 2020 by Qu Yi <q2020y@foxmail.com>
%% This work may be distributed and/or modified under the 
%% conditions of the CC-BY-NC 4.0 License.
%% Gitee: https://gitee.com/TeXibook/ibook
%%
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ibook}[2020/12/04 v2.01]
\RequirePackage{kvoptions,etoolbox}
\SetupKeyvalOptions{family=ibook,prefix=@ibook@,setkeys=\kvsetkeys}
\newcommand\ikv[1]{\kvsetkeys{ibook}{#1}}

\DeclareBoolOption[false]{draftmark}
\DeclareBoolOption[false]{nofont}
\DeclareBoolOption[false]{withpart}
\DeclareBoolOption[false]{withchap}
\DeclareBoolOption[false]{multoc}
\DeclareBoolOption[false]{newline}
\DeclareBoolOption[false]{enroco}
\DeclareBoolOption[false]{article}
\DeclareBoolOption[true]{myfont}
\DeclareComplementaryOption{ctexfont}{myfont}
\DeclareBoolOption[true]{cjktitle}
\DeclareComplementaryOption{entitle}{cjktitle}
\DeclareBoolOption[true]{chinese}
\DeclareComplementaryOption{english}{chinese}
\DeclareStringOption[black]{color}
\DeclareVoidOption{black}{\ikv{color=black}}
\DeclareVoidOption{seagreen}{\ikv{color=seagreen}}
\DeclareStringOption[a4]{geometry}
\DeclareVoidOption{a4}{\ikv{geometry=a4}}
\DeclareVoidOption{b5}{\ikv{geometry=b5}}
\DeclareStringOption[number]{cite}
\DeclareVoidOption{number}{\ikv{cite=number}}
\DeclareVoidOption{authoryear}{\ikv{cite=authoryear}}
\DeclareDefaultOption{\PassOptionsToClass{\CurrentOption}{ctexbook}}
\ProcessKeyvalOptions*\relax

\if@ibook@enroco
	\@ibook@englishtrue
	\@ibook@cjktitlefalse
	\@ibook@newlinetrue
\fi
\if@ibook@chinese
	\PassOptionsToClass{scheme=chinese}{ctexbook}
\else
	\PassOptionsToClass{scheme=plain}{ctexbook}
\fi
\if@ibook@myfont
	\PassOptionsToClass{fontset=none}{ctexbook}
\fi

\LoadClass[UTF8]{ctexbook}

\RequirePackage[svgnames]{xcolor}
\ifdefstring{\@ibook@color}{black}{
	\definecolor{frame}{HTML}{000000}
	\definecolor{one}{HTML}{000000}
	\definecolor{two}{HTML}{000000}
	\definecolor{three}{HTML}{000000}
}{}
\ifdefstring{\@ibook@color}{seagreen}{
	\definecolor{frame}{HTML}{008b8b}
	\definecolor{one}{HTML}{0000cd}
	\definecolor{two}{HTML}{ff0000}
	\definecolor{three}{HTML}{9a1986}
}{}

\RequirePackage{pdfpages}	
\RequirePackage{setspace}	
\RequirePackage{tikz}	
\RequirePackage{mathtools,amsmath,amssymb,amsthm,cases}
\RequirePackage[version=4]{mhchem}	
\RequirePackage{chemfig}	
\RequirePackage{txfonts}
\RequirePackage{siunitx,ulem}
\RequirePackage{hologo}\hologoSetup{discretionarybreak=true}
\RequirePackage{geometry,marginnote}
\RequirePackage{fancyhdr,titletoc}
\RequirePackage{caption,subcaption,rotfloat}	
\RequirePackage{graphicx,wrapfig,duckuments,fontawesome}	
\RequirePackage{tabularx,multirow,makecell,tabu,booktabs}
\RequirePackage{paralist,enumitem}
\RequirePackage[hang,stable]{footmisc}
\RequirePackage[sort&compress]{natbib}
\RequirePackage{listings}
\RequirePackage{imakeidx}\makeindex[intoc]
\RequirePackage{hyperref}
\RequirePackage{cleveref}

\if@ibook@myfont
	\if@ibook@nofont
	\else
		\setmainfont{NotoSerif}[
			Extension=.ttf,
			UprightFont=*-Regular,
			BoldFont=*-Bold,
			ItalicFont=*-Italic]
		\setsansfont{NotoSans}[
			Extension=.ttf,
			UprightFont=*-Regular,
			BoldFont=*-Bold,
			ItalicFont=*-Italic]
		\setmonofont{NotoSansMono}[
			Extension=.ttf,
			UprightFont=*-Regular,
			BoldFont=*-Bold]
		\setCJKmainfont{NotoSerifCJKsc-Regular}[BoldFont=NotoSansHans-Regular,ItalicFont=FZKTJW--GB1-0]
		\setCJKsansfont{NotoSansHans-Regular}[AutoFakeBold=1.5]
		\setCJKmonofont{FZFSJW--GB1-0}
		\newCJKfontfamily[songti]\songti{NotoSerifCJKsc-Regular}[AutoFakeBold=1.5]
		\newCJKfontfamily[heiti]\heiti{NotoSansHans-Regular}
		\newCJKfontfamily[kaishu]\kaishu{FZKTJW--GB1-0}
		\newCJKfontfamily[fangsong]\fangsong{FZFSJW--GB1-0}
	\fi
\fi

\ifdefstring{\@ibook@geometry}{a4}{
	\geometry{
		a4paper,
		columnsep=2\ccwd,
		vmargin=2.54cm,
		hmargin=3.17cm,
		headheight=2.04cm,
		headsep=0.5cm,
		footskip=0.79cm,
		footnotesep=0.5cm,
		marginparsep=8pt,
		marginparwidth=2.54cm
	}}{}
\ifdefstring{\@ibook@geometry}{b5}{
	\geometry{
		b5paper,
		columnsep=2\ccwd,
		margin=2.54cm,
		headheight=2.09cm,
		headsep=0.45cm,
		footskip=0.7cm,
		footnotesep=0.45cm,
		marginparsep=0.25cm,
		marginparwidth=2.25cm
	}}{}

\ctexset{
	part={
		pagestyle=empty,
		number={\if@ibook@cjktitle\chinese{part}\else\thepart\fi},
		format={\color{frame}\raggedright\sffamily\Huge},
		aftername={\if@ibook@newline\par\nobreak\else\hspace{\ccwd}\fi},
		tocline={\CTEXifname{\CTEXthepart\hspace{\ccwd}}{}#2}	
	},
	chapter={
		pagestyle=fancy,
		% pagestyle=empty,
		number={\if@ibook@cjktitle\chinese{chapter}\else\thechapter\fi},
		format={\color{frame}\sffamily\LARGE\if@ibook@newline\else\centering\fi},
		aftername={\if@ibook@newline\par\nobreak\vskip 1.5pc\else\hspace{\ccwd}\fi},
		beforeskip={-1.5ex},
		afterskip={3ex},
		lotskip=0.8pc,
		lofskip=0.8pc,
		tocline={\CTEXifname{\protect\numberline{\CTEXthechapter}}{}#2}	
	},
	section={	
		hang=true,
		format={\color{frame}\raggedright\sffamily\Large},
		aftername={\hspace{0.5\ccwd}},
		beforeskip={1.3ex plus .1ex minus .2ex},
		afterskip={1.3ex plus .2ex minus .1ex},
		tocline={\CTEXifname{\protect\numberline{\CTEXthesection}}{}#2}
	},
	subsection={
		hang=true,
		format={\color{frame}\raggedright\sffamily\large},
		aftername={\hspace{0.5\ccwd}},
		beforeskip={1.3ex plus .1ex minus .2ex},
		afterskip={1.3ex plus .2ex minus .1ex},
		tocline={\CTEXifname{\protect\numberline{\CTEXthesubsection}}{}#2}
	},
	subsubsection={
		hang=true,
		format={\color{frame}\raggedright\sffamily},
		aftername={\hspace{0.5\ccwd}},
		beforeskip={\parskip},
		afterskip={\parskip}
	}
}

\pagestyle{fancy}
\renewcommand\chaptermark[1]{
	\markboth{
		\MakeUppercase{
			\ifnum\c@secnumdepth>\m@ne
				\if@mainmatter
					\CTEXthechapter\hspace{\ccwd}
				\fi
			\fi
		#1}}{
		\MakeUppercase{
			\ifnum\c@secnumdepth>\m@ne
				\if@mainmatter
					\CTEXthechapter\hspace{\ccwd}
				\fi
			\fi
		#1}}}
\renewcommand\sectionmark[1]{
	\markright{
		\ifnum\c@secnumdepth>\z@
			\CTEXthesection\hspace{0.5\ccwd}
		\fi
	#1}}
\fancyhf{} 
\fancyhead[EC]{\color{frame}\kaishu\leftmark}
\fancyhead[OC]{\color{frame}\kaishu\rightmark}
\fancyhead[EL,OR]{\color{frame}\thepage} 
\ifdefstring{\@ibook@language}{chinese}{
	\if@ibook@draftmark
		\fancyfoot[C]{\color{gray!60}\sffamily 未定稿\today}
	\fi}{}
\ifdefstring{\@ibook@language}{english}{
	\if@ibook@draftmark
		\fancyfoot[C]{\color{gray!60}\sffamily Draftmark\space\today}
	\fi}{}
\renewcommand\headrule{
	\color{frame}\vspace{2.5pt}\hrule width\textwidth height 0.75pt}

\linespread{1.3541667}
\setlength{\parskip}{0pt plus 2pt minus 1pt}


\if@ibook@withpart
	\@addtoreset{chapter}{part}
\fi
\if@ibook@newline
\else
	\ctexset{
		part/hang=true,
		chapter/hang=true
	}
\fi

\titlecontents{part}
	[0\ccwd]
	{\addvspace{1pc}\sffamily\large}
	{}
	{}
	{\titlerule*{}{\bf\normalsize\contentspage}}
\titlecontents{chapter}
	[0\ccwd]
	{\addvspace{1pc}\sffamily\large}
	{\thecontentslabel\hspace{\ccwd}}
	{}
	{\titlerule*[0.55pc]{\normalsize\scalebox{1.2}{$\cdot$}}{\bf\normalsize\contentspage}}
\titlecontents{section}
	[1.2\ccwd]
	{\addvspace{0.35pc}}
	{\thecontentslabel\hspace{\ccwd}}
	{}
	{\titlerule*[0.55pc]{\scalebox{1.2}{$\cdot$}}{\contentspage}}
\titlecontents{subsection}
	[3.5\ccwd]
	{\addvspace{0.35pc}}
	{\thecontentslabel\hspace{\ccwd}}
	{}
	{\titlerule*[0.55pc]{\scalebox{1.2}{$\cdot$}}{\contentspage}}	
\titlecontents{figure}
	[0\ccwd]
	{\addvspace{0.2pc}}
	{\thecontentslabel\hspace{\ccwd}}
	{}
	{\titlerule*[0.55pc]{\scalebox{1.2}{$\cdot$}}{\contentspage}}
\titlecontents{table}
	[0\ccwd]
	{\addvspace{0.2pc}}
	{\thecontentslabel\hspace{\ccwd}}
	{}
	{\titlerule*[0.55pc]{\scalebox{1.2}{$\cdot$}}{\contentspage}}

\renewcommand\tableofcontents{
	\if@twocolumn
		\@restonecoltrue\onecolumn
		\if@ibook@multoc
			\twocolumn
		\fi
	\else
		\@restonecolfalse
		\if@ibook@multoc
			\twocolumn
		\fi
	\fi
	\chapter*{\contentsname}
	\markboth{\MakeUppercase\contentsname}{\MakeUppercase\contentsname}
	\addvspace{-1pc}
	\@starttoc{toc}
	\if@restonecol
		\twocolumn
	\else
		\onecolumn
	\fi}
\renewcommand\listoffigures{
	\if@twocolumn
		\@restonecoltrue\onecolumn
		\if@ibook@multoc
			\twocolumn
		\fi
	\else
		\@restonecolfalse
		\if@ibook@multoc
			\twocolumn
		\fi
	\fi
	\chapter*{\phantomsection\listfigurename}
	\addcontentsline{toc}{chapter}{\listfigurename}	
	\markboth{\MakeUppercase\listfigurename}{\MakeUppercase\listfigurename}
	\addvspace{-0.2pc}
	\@starttoc{lof}
	\if@restonecol
		\twocolumn
	\else
		\onecolumn
	\fi}
\renewcommand\listoftables{
	\if@twocolumn
		\@restonecoltrue\onecolumn
		\if@ibook@multoc
			\twocolumn
		\fi
	\else
		\@restonecolfalse
		\if@ibook@multoc
			\twocolumn
		\fi
	\fi	
	\chapter*{\phantomsection\listtablename}
	\addcontentsline{toc}{chapter}{\listtablename}
	\markboth{\MakeUppercase\listtablename}{\MakeUppercase\listtablename}
	\addvspace{-0.2pc}
	\@starttoc{lot}
	\if@restonecol
		\twocolumn
	\else
		\onecolumn
	\fi}

\renewcommand\bibsection{\chapter{\bibname}}
\setlength{\bibsep}{0ex}
\ifdefstring{\@ibook@cite}{number}{
	\setcitestyle{square,comma,super}
	\bibliographystyle{bibstyle/gbt7714-numerical}	
}{}
\ifdefstring{\@ibook@cite}{authoryear}{
	\bibliographystyle{bibstyle/gbt7714-author-year}	
}{}

\DeclareCaptionLabelSeparator{ccwd}{\hspace{\ccwd}}
\DeclareCaptionLabelFormat{parens}{\bothIfFirst{#1}{\space}(#2)}
\captionsetup{
	format=hang,
	% figurewithin=section,
	% tablewithin=section,
	labelfont={sf,small,color=frame},
	labelsep=ccwd,
	textfont={sf,small},
	belowskip=0ex,aboveskip=1.8ex
}
\captionsetup[subfigure]{
	labelfont={sf,small,color=frame},
	labelformat=parens,
	textfont={sf,small},
	belowskip=0.3ex,aboveskip=1.1ex
}
\captionsetup[lstlisting]{singlelinecheck=false}
\renewcommand\thetable{\thechapter\textendash\arabic{table}}
\renewcommand\thesubfigure{\alph{subfigure}}

\setlength{\floatsep}{2.2ex plus .1ex minus .1ex}	
\setlength{\intextsep}{2.6ex plus .1ex minus .1ex}
\setlength{\textfloatsep}{3ex plus .1ex minus .1ex}	
\setlength{\dbltextfloatsep}{3ex plus .1ex minus .1ex}
\setlength{\@fpsep}{8pt plus 2.0fil}	
\setcounter{topnumber}{4}
\setcounter{bottomnumber}{4}
\setcounter{totalnumber}{8}
\renewcommand\textfraction{0.18} 
\renewcommand\topfraction{0.85}	
\renewcommand\bottomfraction{0.65}    
\renewcommand\floatpagefraction{0.60}	

\newcolumntype{L}{X}
\newcolumntype{C}{>{\centering\arraybackslash}X}
\newcolumntype{R}{>{\raggedleft\arraybackslash}X}
\graphicspath{{figures/}}	
\DeclareGraphicsExtensions{.pdf,.png,.jpg,.eps,.tif}

\setlist[itemize,1]{label=\color{three}\ensuremath{\bullet},leftmargin=*}
\setlist[itemize,2]{label=\color{three}\ensuremath{\circ},leftmargin=\ccwd}
\setlist[enumerate,1]{label=\color{three}\arabic*.,ref=\arabic*,leftmargin=*}
\setlist[enumerate,2]{label=\color{three}\roman*.,ref=\theenumi.\roman*,leftmargin=1.5\ccwd}
\setlist[description]{font=\color{three}\sf,leftmargin=0\ccwd}
\setlist{
	labelsep=0.5\ccwd,listparindent=2\ccwd,
	itemsep=.75ex plus .1ex,parsep=0ex,topsep=.75ex plus .1ex,partopsep=0ex}
\newcommand\circled[1]{
	\lower.7ex
	\hbox{\tikz\draw(0pt,0pt)circle(.5\ccwd)node{\makebox[\ccwd][c]{\small#1}};}}
\setlength{\pltopsep}{.75ex plus .1ex}
\setlength{\plpartopsep}{0ex}
\setlength{\plitemsep}{.75ex plus .1ex}
\setlength{\plparsep}{0ex}

\newtheoremstyle{mystyle}	% Name
	{.75ex plus .1ex}	% Space above
	{.75ex plus .1ex}	% Space below
	{}	% Body font
	{}	% Indent amount
	{\color{one}\sffamily}	% Theorem head font
	{}	% Punctuation after theorem head
	{\ccwd}	% Space after theorem head
	{\thmname{#1\space}\thmnumber{#2}\thmnote{#3}}	% Theorem head spec
\theoremstyle{mystyle}
\newtheorem{theorem}{\theoname}[chapter]
\newtheorem{definition}{\definame}[chapter]
\newtheorem{lemma}{\lemmname}[chapter]
\newtheorem{corollary}{\coroname}[chapter]
\newtheorem{proposition}{\propname}[chapter]
\newtheorem{example}{\examname}[chapter]
\let\proof\relax
\newtheorem*{remark}{\faCommentingO\space\remaname}
\newtheorem*{proof}{\faPencilSquareO\space\prooname}

\allowdisplaybreaks[4]
\AtBeginDocument{
	\setlength{\abovedisplayskip}{5pt plus 1pt minus 1pt}
	\setlength{\belowdisplayskip}{5pt plus 1pt minus 1pt}
	\setlength{\abovedisplayshortskip}{0pt}
	\setlength{\belowdisplayshortskip}{5pt plus 1pt minus 1pt}}
% \numberwithin{equation}{section}
\def\tagform@#1{\maketag@@@{\color{one}(\ignorespaces#1\unskip\@@italiccorr)}}

\setlength\footnotemargin{0.5\ccwd}	
\renewcommand\hangfootparskip{0ex}
\setlength{\footnotesep}{0ex}
\renewcommand\footnoterule{
	\color{frame}\hrule width 0.348\columnwidth height 0.75pt \vspace*{2.6pt}\color{black}}
\if@ibook@chinese
	\renewcommand\hangfootparindent{2\ccwd}	
\fi	
\if@ibook@withpart
\else
	\if@ibook@withchap
	\else
		\@removefromreset{footnote}{chapter}	
		\@addtoreset{footnote}{part}
	\fi	
\fi
\renewcommand\marginfont{\sffamily}

\lstset{
	language=[LaTeX]TeX,
	texcsstyle=*\lst@ifdisplaystyle\bfseries\color{three}\else\color{two}\fi,
	basicstyle=\ttfamily\lst@ifdisplaystyle\small\else\color{two}\fi,
	breaklines=true,	
	numbers=left,		
	numberstyle=\small,	
	framexleftmargin=0pt,
	framexrightmargin=0pt,
	xleftmargin=0pt,
	xrightmargin=0pt,
	abovecaptionskip=-1ex,
	belowcaptionskip=1ex,
	aboveskip=1.5ex plus .2ex minus .1ex,
	belowskip=0.55ex plus .2ex minus .1ex,
	keepspaces=true,
	frame=lines,
	framerule=0.5pt,
	columns=flexible,
	rulecolor=\color{two},
	commentstyle=\color{gray},
	emphstyle={\color{one}},
	emph={english,chinese,enroco,entitle,authoryear,aligned,align,bibstyle,nofont,number,Unicode,imakeidx,cite,geometry,color,openany,oneside,twoside,onecolumn,twocolumn,kvoptions,footnotemargin,hang,figure,table,cleveref,footmisc,siunitx,mhchem,chemfig,tabularx,booktabs,tabular,LaTeX,Python,word,ctex,xeCJK,ctexbook,ctexart,book,article,listings,amsthm,amsmath,natbib,caption,subcaption,txfonts,subfig,ntheorem,tabu,newline,titlesec,tocloft,titletoc,split,cases,subnumcases,multline,paralist,inparaenum,inparaitem,compactenum,compactitem,enumerate,itemize,description,enumitem,multoc,withpart,withchap,super,ulem},
	keywordstyle=\lst@ifdisplaystyle\bfseries\color{three}\else\color{two}\fi,
	morekeywords={includegraphics,setmainfont,setsansfont,setmonofont,setCJKmainfont,setCJKsansfont,setCJKmonofont,setCJKfamilyfont,RequirePackage,maketitle,keywords,definecolor,appendix,part,chapter,hypersetup,tableofcontents,songti,heiti,kaishu,fangsong,marginnote,textbf}}

\hypersetup{
	colorlinks,
	% hidelinks,
	linkcolor=one,
	anchorcolor=one,
	citecolor=one,
	urlcolor=one,
	pdfstartview=FitH,	
	pdftitle={},
	pdfsubject={ibook}
}
\urlstyle{tt}
\RequirePackage[
	lang={chinese-utf8},
	type={CC},
	modifier={by-nc},
	version={4.0}
	]{doclicense}

\if@ibook@article
	\renewcommand\thesection{\@arabic\c@section}
	\renewcommand\thetable{\@arabic\c@table}
	\renewcommand\thefigure{\@arabic\c@figure}
	\renewcommand\theequation{\@arabic\c@equation}
	\lstset{numberbychapter=false}
\fi

\if@ibook@chinese
	\renewcommand\contentsname{目\hspace{\ccwd}录}
	\renewcommand\indexname{索\hspace{\ccwd}引}
	\renewcommand\listfigurename{图片索引}
	\renewcommand\listtablename{表格索引}
	\renewcommand\lstlistingname{\faCode 代码}
	\newcommand\theoname{定理}
	\newcommand\definame{定义}
	\newcommand\lemmname{引理}
	\newcommand\coroname{推论}
	\newcommand\propname{性质}
	\newcommand\examname{例}
	\newcommand\remaname{注}
	\newcommand\prooname{证明}
	\crefformat{equation}{#2式（#1）#3}
	\crefformat{align}{#2式（#1）#3}
	\crefformat{multline}{#2式（#1）#3}
	\crefformat{gather}{#2式（#1）#3}
	\crefformat{figure}{#2图#1#3}
	\crefformat{table}{#2表#1#3}
	\crefformat{chapter}{#2第#1章#3}
	\crefformat{section}{#2节#1#3}
	\crefformat{subsection}{#2小节#1#3}
	\crefformat{theorem}{#2定理#1#3}
	\crefformat{definition}{#2定义#1#3}
	\crefformat{lemma}{#2引理#1#3}
	\crefformat{corollary}{#2推论#1#3}
	\crefformat{proposition}{#2性质#1#3}
	\crefformat{example}{#2例#1#3}
\else
	\renewcommand\lstlistingname{\faCode\space Code}
	\newcommand\theoname{Theorem}
	\newcommand\definame{Definition}
	\newcommand\lemmname{Lemma}
	\newcommand\coroname{Corollary}
	\newcommand\propname{Proposition}
	\newcommand\examname{Example}
	\newcommand\remaname{Remark}
	\newcommand\prooname{Proof}
	\crefformat{equation}{#2Equation\space(#1)#3}
	\crefformat{align}{#2Equation\space(#1)#3}
	\crefformat{multline}{#2Equation\space(#1)#3}
	\crefformat{gather}{#2Equation\space(#1)#3}
	\crefformat{figure}{#2Figure\space#1#3}
	\crefformat{table}{#2Table\space#1#3}
	\crefformat{chapter}{#2Chapter\space#1#3}
	\crefformat{section}{#2Section\space#1#3}
	\crefformat{subsection}{#2Subsection\space#1#3}
	\crefformat{theorem}{#2Theorem\space#1#3}
	\crefformat{definition}{#2Definition\space#1#3}
	\crefformat{lemma}{#2Lemma\space#1#3}
	\crefformat{corollary}{#2Corollary\space#1#3}
	\crefformat{proposition}{#2Proposition\space#1#3}
	\crefformat{example}{#2Example\space#1#3}
\fi
%%
%% End of file `ibook.cls'.