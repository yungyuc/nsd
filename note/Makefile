PYTHON ?= python3
SHELL = /bin/bash
NUM ?= 3 # For LaTeX to correctly cross-reference.  To save time, use 1.
VERBOSE ?= 0
DOEPS ?= 1
ifeq ($(VERBOSE),0)
	CMDLOG_DIR_REDIRECT := > /dev/null
else
	CMDLOG_DIR_REDIRECT := >&1
endif
PSTAKE := ./pstake.py

PYTHONPATH := $(abspath ../spacetime):$(PYTHONPATH)
export PYTHONPATH

# tool binaries
LATEX := latex -halt-on-error -interaction=nonstopmode -synctex=1
BIBTEX := bibtex
DVIPS := dvips
PS2PDF := ps2pdf
ifeq ($(DOEPS),0)
	DO_PSTAKE = mkdir -p $(EPS_DIR) ; touch $@
	DO_PSPY = mkdir -p $(EPS_DIR) ; touch $@
else
	DO_PSTAKE = mkdir -p $(EPS_DIR) ; $(PYTHON) $(PSTAKE) $< $@
	DO_PSPY = mkdir -p $(EPS_DIR) ; $(PYTHON) $< $@
endif

# tool binaries

# directories
LOG_DIR := log
EPS_DIR := turgon_eps
SCHEMATIC_DIR := schematic

HANDOVER_DIR := handover
NOWDATE := $(shell date +"%Y%m%d")
CURRENTBRANCH := $(shell git rev-parse --abbrev-ref HEAD 2> /dev/null || echo "nobranch")
NOWID := $(shell git rev-parse --short HEAD 2> /dev/null || echo "nogit")
HANDOVER_FN = $(HANDOVER_DIR)/$(basename $<).$(CURRENTBRANCH).$(NOWID).$(NOWDATE).pdf

TRUNK := turgon

MAKEFILES := Makefile

ALL_TEX := $(wildcard $(SCHEMATIC_DIR)/*.tex)
ALL_SCHPY := $(wildcard $(SCHEMATIC_DIR)/*.py)
ALL_EPS := $(patsubst $(SCHEMATIC_DIR)/%.tex,$(EPS_DIR)/%.eps,$(ALL_TEX)) \
	$(patsubst $(SCHEMATIC_DIR)/%.py,$(EPS_DIR)/%.eps,$(ALL_SCHPY))

.PHONY: default
default: syllabus

.PHONY: eps
eps: $(ALL_EPS)

$(EPS_DIR)/%.eps: $(SCHEMATIC_DIR)/%.tex pstake.py
	mkdir -p $(LOG_DIR) ; \
	$(DO_PSTAKE) 2>&1 | \
		tee $(LOG_DIR)/pstake-$(notdir $<).log $(CMDLOG_DIR_REDIRECT)

$(EPS_DIR)/%.eps: $(SCHEMATIC_DIR)/%.py
	mkdir -p $(LOG_DIR) ; \
	$(DO_PSPY) 2>&1 | \
		tee $(LOG_DIR)/$(notdir $<).log $(CMDLOG_DIR_REDIRECT)

$(EPS_DIR)/%.png: $(SCHEMATIC_DIR)/%.py
	mkdir -p $(LOG_DIR) ; \
	$(DO_PSPY) 2>&1 | \
		tee $(LOG_DIR)/$(notdir $<).log $(CMDLOG_DIR_REDIRECT)

%.dvi: %.tex $(ALL_EPS) $(MAKEFILES)
	@echo "Having EPS files: $(ALL_EPS)"
	mkdir -p $(LOG_DIR) ; num=1 ; while [ $$num -le $(NUM) ] ; do \
		latex_logfile=$(LOG_DIR)/$@.latex.$$num.log ; \
		echo -n "latex $@ #$$num ... " ; \
		$(LATEX) $< 2>&1 | \
			tee $$latex_logfile $(CMDLOG_DIR_REDIRECT) ; \
		if [[ \
			   ( -z "`grep 'LaTeX Error:' $$latex_logfile`" ) \
			&& ( -z "`grep 'Runaway argument\?' $$latex_logfile`" ) \
			&& ( -z "`grep \! $$latex_logfile | head -c 1 | grep \!`" ) \
		]] ; then \
			echo "done" ; \
		else \
			echo "tail $$latex_logfile:" ; \
			tail $$latex_logfile ; \
			exit -1 ; \
		fi ; \
		$(BIBTEX) $(basename $<) 2>&1 | \
			tee $(LOG_DIR)/$@.bibtex.$$num.log \
			$(CMDLOG_DIR_REDIRECT) ; \
	(( num = num + 1 )) ; done

%.ps: %.dvi $(MAKEFILES)
	mkdir -p $(LOG_DIR) ; \
	$(DVIPS) $< 2>&1 | tee $(LOG_DIR)/$@.log $(CMDLOG_DIR_REDIRECT)

%.pdf: %.ps $(MAKEFILES)
	mkdir -p $(LOG_DIR) ; \
	$(PS2PDF) $< 2>&1 | tee $(LOG_DIR)/$@.log $(CMDLOG_DIR_REDIRECT)

.PHONY: syllabus
syllabus: syllabus.pdf

.PHONY: clean_tex
clean_tex:
	rm -f *.aux *.bbl *.blg *.dvi *.log *.out *.xwm *.toc *.pdfsync *.synctex.gz *.pdf *.ps

.PHONY: clean_log
clean_log:
	rm -rf $(LOG_DIR)

.PHONY: clean_eps
clean_eps:
	rm -rf $(EPS_DIR)

.PHONY: clean
clean: clean_tex clean_log clean_eps
