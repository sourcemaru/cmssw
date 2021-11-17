# Set M0 as default HCAL reconstruction over full HBHE Digi range
# To be used in cmsDriver command: 
# --customise RecoLocalCal/HcalRecProducers/M0defaultFullRange_cff.customiseM0

import FWCore.ParameterSet.Config as cms
def customiseM0(process):
    if hasattr(process,'hbhereco'):
       process.hbhereco.cpu.tsFromDB = False
       process.hbhereco.cpu.recoParamsFromDB = False
       process.hbhereco.cpu.sipmQTSShift = -99
       process.hbhereco.cpu.sipmQNTStoSum = 99
       process.hbhereco.cpu.algorithm.useMahi = False
       process.hbhereco.cpu.algorithm.useM2 = False
       process.hbhereco.cpu.algorithm.useM3 = False
       process.hbhereco.cpu.algorithm.correctForPhaseContainment = False
       process.hbhereco.cpu.algorithm.firstSampleShift = -999
       process.hbhereco.cpu.algorithm.samplesToAdd = 10

    return(process)
