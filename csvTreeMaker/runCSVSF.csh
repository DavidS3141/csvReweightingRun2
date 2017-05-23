#!/bin/csh -f

set isCSV = 1
if ($isCSV != 1) then
    set tagger = "cMVA"
else 
    set tagger = "csv"
endif 

## run on Data samples
root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'('$isCSV', 1, 0, "", -100)'
root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'('$isCSV', 1, 0, "", -200)'
root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'('$isCSV', 1, 0, "", -300)'

root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'('$isCSV', 0, 0, "", -100)'
root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'('$isCSV', 0, 0, "", -200)'
root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'('$isCSV', 0, 0, "", -300)'

set postFix = "" ### change
## Iterations (three seems to be sufficent)
foreach jes ( "" "JESTimePtEtaup" "JESTimePtEtadown" "JESFlavorQCDup" "JESFlavorQCDdown" )#"JESUp" "JESDown") 
    echo "----->start running for $jes ----------="
    foreach iter ( 0 )#1 2 )#3 )
	echo "   ----->start iteration version $iter ----------="

	foreach sample( 2500 2300 2310 2514 2515 2600 )# 
#	foreach sample( 2300 2310  )# 

	    root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'('$isCSV', 1, '$iter', "'$jes'", '$sample')'
	    root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'('$isCSV', 0, '$iter', "'$jes'", '$sample')'

	end

	if ($iter < 3) then ## change the argument
	    echo "   ----->start hadd all samples ----------="
	    ## hadd all samples
	    
	    if ($iter == 0 && $jes == "") then
		hadd -f macros/{$tagger}_rwt_hf_all_v{$iter}{$jes}{$postFix}.root CSVHistoFiles/{$tagger}_rwt_hf_*_v{$iter}{$jes}_histo{$postFix}.root
		hadd -f macros/{$tagger}_rwt_lf_all_v{$iter}{$jes}{$postFix}.root CSVHistoFiles/{$tagger}_rwt_lf_*_v{$iter}{$jes}_histo{$postFix}.root
	    else
		    hadd -f macros/{$tagger}_rwt_hf_all_v{$iter}{$jes}{$postFix}.root CSVHistoFiles/{$tagger}_rwt_hf_*_v{$iter}{$jes}_histo{$postFix}.root CSVHistoFiles/{$tagger}_rwt_hf_MuonEG_v0_histo{$postFix}.root CSVHistoFiles/{$tagger}_rwt_hf_DoubleEG_v0_histo{$postFix}.root CSVHistoFiles/{$tagger}_rwt_hf_DoubleMuon_v0_histo{$postFix}.root
		    hadd -f macros/{$tagger}_rwt_lf_all_v{$iter}{$jes}{$postFix}.root CSVHistoFiles/{$tagger}_rwt_lf_*_v{$iter}{$jes}_histo{$postFix}.root CSVHistoFiles/{$tagger}_rwt_lf_DoubleEG_v0_histo{$postFix}.root CSVHistoFiles/{$tagger}_rwt_lf_DoubleMuon_v0_histo{$postFix}.root CSVHistoFiles/{$tagger}_rwt_lf_MuonEG_v0_histo{$postFix}.root
	    endif
	    
	    ## Do LF and HF fitting
	    echo "   ----->start fitting SFs ----------="
	    cd macros/
	    root -b -q head13TeV.C fitHF_csvSF_13TeV.C'('$isCSV', "'$tagger'_rwt_hf_all_v'$iter''$jes''$postFix'.root", '$iter', "'$jes'", "v'$iter''$jes'")'
	    root -b -q head13TeV.C fitLF_csvSF_13TeV.C'('$isCSV', "'$tagger'_rwt_lf_all_v'$iter''$jes''$postFix'.root", '$iter', "'$jes'", "v'$iter''$jes'")'
	    
	    echo "   ----->copy SFs to data/----------="
	    cp {$tagger}_rwt_fit_hf_v{$iter}{$jes}.root  ../data/
	    cp {$tagger}_rwt_fit_lf_v{$iter}{$jes}.root  ../data/
	    cd -
	endif
    end

end


echo "Finished runCSVSF.csh"

#if( $#argv < 7 ) then
#    echo "usage ./runCSVSF.csh [POSTFIX] [POIS_FLUCT] [USE_8TEV_SF] [P0_HF] [P1_HF] [P0_LF] [P1_LF]"
#    exit 2
#endif
#
#set dyname="DYJetsToLL_M_50_13TeV_madgraph_pythia8_PU20bx25_POSTLS170_V5_v1"
#set ttname="TTJets_MSDecaysCKM_central_Tune4C_13TeV_madgraph_PU20bx25_POSTLS170_V5_v1"
#set postfit=$1
#set usePois=$2
#set use8TeVSF=$3
#set p0_HF=$4
#set p1_HF=$5
#set p0_LF=$6
#set p1_LF=$7
#
#if( $#argv>2 ) then
#    set lumi = $8
#else
#    set lumi = -1
#endif
#
#
#set dir="Images"
#if( ! -e $dir ) then
#    mkdir $dir
#endif
#

##	root -b -q macros/head13TeV.C macros/csvSF_treeReader_13TeV.C+'(1,'$iter', "", '$sample', -1,1,1,'$lumi')'
##    hadd -f macros/csv_rwt_hf_all_v2_histo.root CSVHistoFiles/csv_rwt_hf_*_v2_histo.root

#	root -b -q macros/head13TeV.C macros/fitHF_csvSF_13TeV.C+'("csv_rwt_hf_v'$iter'_histo.root",'$iter',"_pseudoData_'$postfit'_iter'$iter'")'
#	root -b -q macros/head13TeV.C macros/fitLF_csvSF_13TeV.C+'("csv_rwt_lf_v'$iter'_histo.root",'$iter',"_pseudoData_'$postfit'_iter'$iter'")'

## Make stacked plots
#root -b -q macros/head13TeV.C macros/makePlots_csvSF_13TeV.C+'("csv_rwt_hf_v0_histo.root",true,"_'$postfit'",true)'
#root -b -q macros/head13TeV.C macros/makePlots_csvSF_13TeV.C+'("csv_rwt_lf_v0_histo.root",false,"_'$postfit'",true)'

