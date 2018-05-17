import logging
import os.path

from Vispa.Main.Directories import pluginDirectory
from Vispa.Plugins.EdmBrowser.EventContentView import EventContentView
from Vispa.Plugins.EdmBrowser.EventContentDataAccessor import EventContentDataAccessor

from PyQt4.QtCore import QCoreApplication, Qt, SIGNAL
from PyQt4.QtGui import QDialog, QVBoxLayout, QHBoxLayout, QPushButton, QMessageBox, QFileDialog

class EventContentDialog(QDialog):
    """ This dialog allows to compare the configuration input/output with dataformats"""
    def __init__(self, parent=None, helpMessage=""):
        logging.debug(__name__ + ": __init__")
        QDialog.__init__(self, parent)
        self._helpMessage=helpMessage
        self._inputFileName=None
        self._configDataAccessor=None
        self._eventContent=None
        self.resize(800, 500)
        self.setWindowFlags(Qt.Window)
        self.setWindowTitle("Browse event content...")
        self.fill()
        self._dataAccessor = EventContentDataAccessor()
        self._eventContentView.setDataAccessor(self._dataAccessor)

    def fill(self):
        logging.debug(__name__ +': fill')
        self.setLayout(QVBoxLayout())
        self._eventContentView=EventContentView(self)
        self.layout().addWidget(self._eventContentView)

        bottom=QHBoxLayout()
        self.layout().addLayout(bottom)
        input=QPushButton("&Select input file")
        bottom.addWidget(input)
        self.connect(input, SIGNAL('clicked()'), self.selectInputFile)
        help=QPushButton("&Help")
        bottom.addWidget(help)
        self.connect(help, SIGNAL('clicked()'), self.help)
        bottom.addStretch()
        ok=QPushButton("&Ok")
        bottom.addWidget(ok)
        ok.setDefault(True)
        self.connect(ok, SIGNAL('clicked()'), self.accept)

    def setConfigDataAccessor(self, accessor):
        self._configDataAccessor=accessor
        self.updateContent()
    
    def setEventContent(self, name, branches):
        self._eventContent=(name, branches)
        self.updateContent()
    
    def updateContent(self):
        self._dataAccessor.clear()
        if self._inputFileName!=None:
            self._dataAccessor.addContentFile(self._inputFileName)
        else:
            self._dataAccessor.addBranches("AODSIM_5_3_6",
	    [['recoGenParticles', 'genParticles', '', 'SIM', '105046', '17162.6'], ['CaloTowersSorted', 'towerMaker', '', 'RECO', '39436.4', '13456.3'], ['recoTracks', 'generalTracks', '', 'RECO', '51294.9', '12567.2'], ['recoPFCandidates', 'particleFlow', '', 'RECO', '53086.8', '9202.08'], ['EcalRecHitsSorted', 'reducedEcalRecHitsEB', '', 'RECO', '13315.9', '6320.85'], ['triggerTriggerEvent', 'hltTriggerSummaryAOD', '', 'HLT', '20729.6', '4619.86'], ['EcalRecHitsSorted', 'reducedEcalRecHitsEE', '', 'RECO', '4692.26', '2404'], ['recoTrackExtrapolations', 'trackExtrapolator', '', 'RECO', '5947.74', '2108.37'], ['recoPFTaus', 'hpsTancTaus', '', 'RECO', '6032.86', '1915.43'], ['recoPFTaus', 'hpsPFTauProducer', '', 'RECO', '6037.15', '1913.58'], ['recoConversions', 'allConversions', '', 'RECO', '8522.69', '1826.85'], ['recoCaloClusters', 'hybridSuperClusters', 'hybridBarrelBasicClusters', 'RECO', '7716.58', '1602.15'], ['recoSuperClusters', 'correctedHybridSuperClusters', '', 'RECO', '5931.16', '1264.82'], ['recoGenJets', 'ak7GenJets', '', 'SIM', '5547.06', '1250.68'], ['recoGenJets', 'kt6GenJets', '', 'SIM', '5475.84', '1247.35'], ['recoPFTaus', 'shrinkingConePFTauProducer', '', 'RECO', '5845.13', '1228.77'], ['recoCaloClusters', 'multi5x5SuperClusters', 'multi5x5EndcapBasicClusters', 'RECO', '3586.54', '1170.02'], ['recoPFJets', 'kt6PFJets', '', 'RECO', '4816.83', '1133.39'], ['recoGenJets', 'ak5GenJets', '', 'SIM', '4800.24', '1126.56'], ['recoPFJets', 'ak7PFJets', '', 'RECO', '4817.79', '1100.64'], ['recoGenJets', 'kt4GenJets', '', 'SIM', '4480.19', '1062.92'], ['recoMuons', 'muons', '', 'RECO', '2005.61', '1061.07'], ['recoPFJets', 'ak5PFJets', '', 'RECO', '4493.1', '1050.59'], ['CastorRecHitsSorted', 'castorreco', '', 'RECO', '4521.27', '1040.24'], ['recoPFJets', 'kt4PFJets', '', 'RECO', '4404.07', '1015.09'], ['TrackingRecHitsOwned', 'standAloneMuons', '', 'RECO', '2793.45', '994.789'], ['TrackingRecHitsOwned', 'refittedStandAloneMuons', '', 'RECO', '2784.12', '991.704'], ['recoGsfTracks', 'electronGsfTracks', '', 'RECO', '3142.86', '961.403'], ['recoGsfElectrons', 'gsfElectrons', '', 'RECO', '1649.21', '917.22'], ['recoPhotons', 'photons', '', 'RECO', '1395.53', '909.15'], ['recoJPTJets', 'TCTauJetPlusTrackZSPCorJetAntiKt5', '', 'RECO', '3352.41', '877.154'], ['recoJPTJets', 'JetPlusTrackZSPCorJetAntiKt5', '', 'RECO', '3350.52', '877.07'], ['recoCaloTaus', 'caloRecoTauProducer', '', 'RECO', '1717.51', '872.849'], ['recoCaloJets', 'kt6CaloJets', '', 'RECO', '2839.9', '807.436'], ['recoCaloJets', 'ak7CaloJets', '', 'RECO', '2821.17', '794.456'], ['recoCaloJets', 'ak5CaloJets', '', 'RECO', '2682.35', '781.914'], ['recoCaloJets', 'kt4CaloJets', '', 'RECO', '2698.33', '778.863'], ['recoMuons', 'muonsFromCosmics', '', 'RECO', '1525.58', '695.097'], ['recoPFCandidates', 'particleFlowTmp', 'electrons', 'RECO', '3150.02', '684.198'], ['recoSuperClusters', 'correctedMulti5x5SuperClustersWithPreshower', '', 'RECO', '2440.26', '641.781'], ['L1GlobalTriggerObjectMaps', 'l1L1GtObjectMap', '', 'RECO', '4817.48', '636.705'], ['recoDeDxDataedmValueMap', 'dedxDiscrimASmi', '', 'RECO', '1576.32', '624.676'], ['recoConversions', 'uncleanedOnlyAllConversions', '', 'RECO', '2609.33', '609.023'], ['recoDeDxDataedmValueMap', 'dedxHarmonic2', '', 'RECO', '1576.14', '586.433'], ['EcalRecHitsSorted', 'reducedEcalRecHitsES', '', 'RECO', '1290.6', '545.354'], ['recoJetIDedmValueMap', 'kt4JetID', '', 'RECO', '1059.21', '510.092'], ['recoJetIDedmValueMap', 'ak5JetID', '', 'RECO', '1021.89', '503.02'], ['recoJetIDedmValueMap', 'kt6JetID', '', 'RECO', '985.287', '499.977'], ['recoJetIDedmValueMap', 'ak7JetID', '', 'RECO', '963.224', '495.082'], ['recoJetPiZeroAssociation', 'ak5PFJetsRecoTauPiZeros', '', 'RECO', '2317.55', '417.197'], ['HBHERecHitsSorted', 'reducedHcalRecHits', 'hbhereco', 'RECO', '643.273', '395.347'], ['recoGsfElectrons', 'uncleanedOnlyGsfElectrons', '', 'RECO', '924.909', '383.082'], ['recoTracks', 'conversionStepTracks', '', 'RECO', '1346.05', '372.365'], ['recoPreshowerClusters', 'multi5x5SuperClustersWithPreshower', 'preshowerYClusters', 'RECO', '2146.15', '370.771'], ['recoJetedmRefToBaseProdTorecoJetExtendedAssociationJetExtendedDatasAssociationVector', 'kt4JetExtender', '', 'RECO', '493.753', '354.089'], ['recoJetedmRefToBaseProdTorecoJetExtendedAssociationJetExtendedDatasAssociationVector', 'ak5JetExtender', '', 'RECO', '476.393', '337.586'], ['recoVertexs', 'offlinePrimaryVertices', '', 'RECO', '3159.91', '335.666'], ['recoVertexs', 'offlinePrimaryVerticesWithBS', '', 'RECO', '3160.08', '335.628'], ['recoRecoTauPiZeros', 'hpsPFTauProducer', 'pizeros', 'RECO', '1237.93', '332.381'], ['recoRecoTauPiZeros', 'hpsTancTaus', 'pizeros', 'RECO', '1236.55', '331.566'], ['recoPreshowerClusters', 'multi5x5SuperClustersWithPreshower', 'preshowerXClusters', 'RECO', '1895.18', '326.838'], ['recoTrackJets', 'ak5TrackJets', '', 'RECO', '1472.57', '325.921'], ['recoMuons', 'muonsFromCosmics1Leg', '', 'RECO', '761.942', '322.608'], ['recoIsoDepositedmValueMap', 'muIsoDepositCalByAssociatorTowers', 'ecal', 'RECO', '514.554', '314.425'], ['recoJetedmRefToBaseProdTorecoJetExtendedAssociationJetExtendedDatasAssociationVector', 'ak7JetExtender', '', 'RECO', '449.109', '309.2'], ['recoIsoDepositedmValueMap', 'muons', 'ecal', 'RECO', '506.881', '308.659'], ['recoRecoTauPiZeros', 'shrinkingConePFTauProducer', 'pizeros', 'RECO', '1080.57', '299.144'], ['recoCaloMETs', 'metHO', '', 'RECO', '431.056', '296.87'], ['recoCaloMETs', 'met', '', 'RECO', '430.513', '295.652'], ['recoTracks', 'ckfInOutTracksFromConversions', '', 'RECO', '1068.5', '278.824'], ['recoCaloMETs', 'caloMetM', '', 'RECO', '466.318', '274.655'], ['recoPhotons', 'pfPhotonTranslator', 'pfphot', 'RECO', '634.161', '265.915'], ['recoIsoDepositedmValueMap', 'muIsoDepositTk', '', 'RECO', '420.616', '252.032'], ['recoIsoDepositedmValueMap', 'muons', 'muIsoDepositTk', 'RECO', '417.133', '250.47'], ['recoSuperClusters', 'pfElectronTranslator', 'pf', 'RECO', '677.557', '242.129'], ['EBDigiCollection', 'selectDigi', 'selectedEcalEBDigiCollection', 'RECO', '621.874', '241.995'], ['recoConversions', 'conversions', '', 'RECO', '938.816', '241.487'], ['recoCaloMETs', 'metNoHF', '', 'RECO', '431.6', '234.794'], ['recoIsoDepositedmValueMap', 'muIsoDepositCalByAssociatorTowers', 'hcal', 'RECO', '365.177', '219.601'], ['recoCaloClusters', 'pfElectronTranslator', 'pf', 'RECO', '572.042', '218.052'], ['recoIsoDepositedmValueMap', 'muons', 'hcal', 'RECO', '357.525', '213.87'], ['recoVertexCompositeCandidates', 'generalV0Candidates', 'Kshort', 'RECO', '566.447', '212.891'], ['recoTrackExtras', 'globalMuons', '', 'RECO', '514.617', '202.63'], ['recoTrackExtras', 'refittedStandAloneMuons', '', 'RECO', '477.334', '199.739'], ['recoTrackExtras', 'standAloneMuons', '', 'RECO', '477.397', '198.785'], ['recoTrackExtras', 'tevMuons', 'default', 'RECO', '384.114', '198.682'], ['recoTrackExtras', 'tevMuons', 'firstHit', 'RECO', '375.837', '198.517'], ['recoTrackExtras', 'tevMuons', 'dyt', 'RECO', '384.357', '198.243'], ['recoTrackExtras', 'tevMuons', 'picky', 'RECO', '383.037', '198.098'], ['recoCaloTauTagInfos', 'caloRecoTauTagInfoProducer', '', 'RECO', '1621.33', '186.79'], ['recoPFMETs', 'pfMet', '', 'RECO', '286.889', '178.238'], ['HcalNoiseSummary', 'hcalnoise', '', 'RECO', '359.4', '161.108'], ['recoTracks', 'cosmicMuons', '', 'RECO', '448.081', '139.641'], ['recoTracks', 'tevMuons', 'default', 'RECO', '371.83', '136.023'], ['recoTracks', 'tevMuons', 'picky', 'RECO', '371.433', '135.368'], ['recoTracks', 'globalMuons', '', 'RECO', '371.08', '135.22'], ['recoTracks', 'standAloneMuons', '', 'RECO', '382.657', '130.57'], ['recoTracks', 'refittedStandAloneMuons', '', 'RECO', '380.804', '129.913'], ['recoTracks', 'tevMuons', 'firstHit', 'RECO', '372.029', '129.526'], ['recoTracks', 'refittedStandAloneMuons', 'UpdatedAtVtx', 'RECO', '380.75', '128.684'], ['recoTracks', 'standAloneMuons', 'UpdatedAtVtx', 'RECO', '381.691', '128.283'], ['recoTracks', 'ckfOutInTracksFromConversions', '', 'RECO', '434.356', '127.853'], ['recoTracks', 'tevMuons', 'dyt', 'RECO', '371.035', '127.538'], ['ints', 'genParticles', '', 'SIM', '3270.52', '127.297'], ['Level1TriggerScalerss', 'scalersRawToDigi', '', 'RECO', '312.121', '121.796'], ['L1GlobalTriggerReadoutRecord', 'gtDigis', '', 'RECO', '1954.5', '121.44'], ['recoPFCandidates', 'particleFlowTmp', 'CleanedPunchThroughNeutralHadrons', 'RECO', '301.285', '120.463'], ['recoVertexCompositeCandidates', 'generalV0Candidates', 'Lambda', 'RECO', '297.311', '119.95'], ['recoJetedmRefToBaseProdrecoTracksrecoTrackrecoTracksTorecoTrackedmrefhelperFindUsingAdvanceedmRefVectorsAssociationVector', 'ak5JetTracksAssociatorAtVertex', '', 'RECO', '449.436', '119.886'], ['recoPFCandidates', 'particleFlowTmp', 'CleanedTrackerAndGlobalMuons', 'RECO', '298.749', '117.894'], ['edmTriggerResults', 'TriggerResults', '', 'HLT', '966.71', '115.848'], ['recoPFCandidates', 'particleFlowTmp', 'CleanedPunchThroughMuons', 'RECO', '296.639', '115.788'], ['ClusterSummary', 'clusterSummaryProducer', '', 'RECO', '300.433', '114'], ['recoPFCandidates', 'particleFlowTmp', 'AddedMuonsAndHadrons', 'RECO', '294.632', '113.762'], ['recoPFCandidates', 'particleFlowTmp', 'CleanedCosmicsMuons', 'RECO', '294.057', '113.203'], ['recoPFCandidates', 'particleFlowTmp', 'CleanedFakeMuons', 'RECO', '292.531', '111.671'], ['EEDigiCollection', 'selectDigi', 'selectedEcalEEDigiCollection', 'RECO', '226.419', '110.283'], ['recoJetedmRefToBaseProdrecoTracksrecoTrackrecoTracksTorecoTrackedmrefhelperFindUsingAdvanceedmRefVectorsAssociationVector', 'ak7JetTracksAssociatorAtVertex', '', 'RECO', '407.474', '109.613'], ['recoPFCandidates', 'particleFlowTmp', 'CleanedHF', 'RECO', '288.895', '108.054'], ['LumiScalerss', 'scalersRawToDigi', '', 'RECO', '280.094', '98.317'], ['recoConversions', 'pfPhotonTranslator', 'pfphot', 'RECO', '290.037', '89.1082'], ['L1TriggerScalerss', 'scalersRawToDigi', '', 'RECO', '230.688', '88.8037'], ['recoPreshowerClusters', 'pfElectronTranslator', 'pf', 'RECO', '511.586', '88.523'], ['recoIsoDepositedmValueMap', 'muIsoDepositJets', '', 'RECO', '151.044', '85.9932'], ['recoIsoDepositedmValueMap', 'muons', 'muIsoDepositJets', 'RECO', '147.637', '84.5297'], ['l1extraL1MuonParticles', 'l1extraParticles', '', 'RECO', '222.075', '82.2001'], ['recoSuperClusters', 'pfPhotonTranslator', 'pfphot', 'RECO', '224.83', '78.5247'], ['GenEventInfoProduct', 'generator', '', 'SIM', '200.096', '77.1556'], ['recoGenMETs', 'genMetCaloAndNonPrompt', '', 'SIM', '225.237', '76.828'], ['recoGlobalHaloData', 'GlobalHaloData', '', 'RECO', '174.237', '75.3417'], ['recoMETs', 'tcMetWithPFclusters', '', 'RECO', '197.039', '74.185'], ['recoMETs', 'htMetIC5', '', 'RECO', '195.501', '72.5763'], ['recoMETs', 'htMetKT6', '', 'RECO', '195.501', '72.57'], ['recoMETs', 'htMetAK5', '', 'RECO', '195.501', '72.566'], ['recoMETs', 'htMetAK7', '', 'RECO', '195.501', '72.5639'], ['recoMETs', 'htMetKT4', '', 'RECO', '195.501', '72.559'], ['recoMETs', 'tcMet', '', 'RECO', '195.082', '72.263'], ['recoMuonTimeExtraedmValueMap', 'muons', 'combined', 'RECO', '136.636', '71.0446'], ['recoGenMETs', 'genMetTrue', '', 'SIM', '225.044', '69.2747'], ['l1extraL1EtMissParticles', 'l1extraParticles', 'MET', 'RECO', '195.589', '67.5822'], ['recoIsoDepositedmValueMap', 'muIsoDepositCalByAssociatorTowers', 'ho', 'RECO', '129.024', '66.7379'], ['l1extraL1EtMissParticles', 'l1extraParticles', 'MHT', 'RECO', '195.589', '66.6076'], ['l1extraL1JetParticles', 'l1extraParticles', 'Tau', 'RECO', '300.882', '64.2213'], ['recoGenMETs', 'genMetCalo', '', 'SIM', '225.044', '63.9482'], ['l1extraL1EmParticles', 'l1extraParticles', 'Isolated', 'RECO', '302.191', '63.5806'], ['recoTracks', 'regionalCosmicTracks', '', 'RECO', '177.98', '62.9744'], ['recoMuonTimeExtraedmValueMap', 'muons', 'dt', 'RECO', '135.677', '62.7767'], ['recoMuonShoweredmValueMap', 'muons', 'muonShowerInformation', 'RECO', '188.862', '62.7092'], ['l1extraL1EmParticles', 'l1extraParticles', 'NonIsolated', 'RECO', '254.756', '61.8529'], ['recoSuperClusters', 'hybridSuperClusters', 'uncleanOnlyHybridSuperClusters', 'RECO', '160.738', '61.8093'], ['l1extraL1JetParticles', 'l1extraParticles', 'Central', 'RECO', '243.356', '61.6566'], ['recoIsoDepositedmValueMap', 'muons', 'ho', 'RECO', '121.375', '61.0219'], ['recoMuonTimeExtraedmValueMap', 'muons', 'csc', 'RECO', '135.801', '59.4212'], ['recoRecoChargedRefCandidates', 'trackRefsForJets', '', 'RECO', '1508.98', '58.4909'], ['recoTracks', 'globalCosmicMuons', '', 'RECO', '148.563', '56.6'], ['BeamSpotOnlines', 'scalersRawToDigi', '', 'RECO', '149.016', '56.4632'], ['recoCastorTowers', 'CastorTowerReco', '', 'RECO', '144.39', '54.935'], ['recoCaloClusters', 'pfPhotonTranslator', 'pfphot', 'RECO', '151.103', '54.8134'], ['recoPhotonCores', 'photonCore', '', 'RECO', '286.784', '54.0343'], ['recoSuperClusters', 'hfEMClusters', '', 'RECO', '154.464', '52.1573'], ['recoTracks', 'uncleanedOnlyCkfInOutTracksFromConversions', '', 'RECO', '122.937', '51.1006'], ['recoTracks', 'uncleanedOnlyCkfOutInTracksFromConversions', '', 'RECO', '121.317', '50.5552'], ['recoPFRecHits', 'particleFlowClusterHFHAD', 'Cleaned', 'RECO', '142.47', '49.4046'], ['recoPFRecHits', 'particleFlowRecHitHCAL', 'Cleaned', 'RECO', '142.461', '49.2331'], ['recoPFRecHits', 'particleFlowClusterECAL', 'Cleaned', 'RECO', '142.22', '49.1331'], ['recoPFRecHits', 'particleFlowClusterHFEM', 'Cleaned', 'RECO', '142.162', '49.089'], ['recoPFRecHits', 'particleFlowClusterHCAL', 'Cleaned', 'RECO', '142.162', '49.089'], ['recoPFRecHits', 'particleFlowRecHitECAL', 'Cleaned', 'RECO', '141.989', '48.8652'], ['recoPFRecHits', 'particleFlowClusterHO', 'Cleaned', 'RECO', '141.763', '48.6902'], ['recoPFRecHits', 'particleFlowClusterPS', 'Cleaned', 'RECO', '141.763', '48.6902'], ['l1extraL1JetParticles', 'l1extraParticles', 'Forward', 'RECO', '147.885', '48.601'], ['recoPFRecHits', 'particleFlowRecHitHO', 'Cleaned', 'RECO', '141.563', '48.5277'], ['recoPFRecHits', 'particleFlowRecHitPS', 'Cleaned', 'RECO', '141.563', '48.5277'], ['recoTracks', 'cosmicMuons1Leg', '', 'RECO', '123.899', '46.279'], ['recoTracks', 'globalCosmicMuons1Leg', '', 'RECO', '117.725', '45.554'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'ghostTrackBJetTags', '', 'RECO', '68.9018', '44.5902'], ['HcalUnpackerReport', 'hcalDigis', '', 'RECO', '277.519', '44.5669'], ['recoCastorJetIDedmValueMap', 'ak7CastorJetID', '', 'RECO', '100.061', '43.7701'], ['HcalUnpackerReport', 'castorDigis', '', 'RECO', '161.781', '43.2439'], ['recoMuonMETCorrectionDataedmValueMap', 'muonTCMETValueMapProducer', 'muCorrData', 'RECO', '80.9984', '42.7349'], ['recoMuonMETCorrectionDataedmValueMap', 'muonMETValueMapProducer', 'muCorrData', 'RECO', '80.834', '42.7064'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'combinedSecondaryVertexMVABJetTags', '', 'RECO', '69.2822', '42.619'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'jetBProbabilityBJetTags', '', 'RECO', '69.0207', '42.5653'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByRawCombinedIsolationDBSumPtCorr', '', 'RECO', '91.2028', '41.5983'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByRawChargedIsolationDBSumPtCorr', '', 'RECO', '91.1774', '40.9131'], ['recoMuonCosmicCompatibilityedmValueMap', 'muons', 'cosmicsVeto', 'RECO', '108.057', '40.6054'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'combinedSecondaryVertexBJetTags', '', 'RECO', '69.2109', '39.0714'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'trackCountingHighEffBJetTags', '', 'RECO', '69.1396', '38.777'], ['recoGsfElectronCores', 'gsfElectronCores', '', 'RECO', '115.731', '36.3584'], ['recoBasicJets', 'ak7BasicJets', '', 'RECO', '101.611', '36.2009'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'jetProbabilityBJetTags', '', 'RECO', '68.9969', '35.9383'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByRawGammaIsolationDBSumPtCorr', '', 'RECO', '91.1268', '35.603'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'trackCountingHighPurBJetTags', '', 'RECO', '69.1396', '35.0703'], ['recoPhotonCores', 'pfPhotonTranslator', 'pfphot', 'RECO', '90.8262', '32.1493'], ['recoRecoEcalCandidates', 'hfRecoEcalCandidate', '', 'RECO', '79.8304', '31.8592'], ['recoHFEMClusterShapes', 'hfEMClusters', '', 'RECO', '74.8034', '31.1033'], ['recoPFCandidateedmPtredmValueMap', 'particleFlow', 'electrons', 'RECO', '77.1247', '31.0379'], ['recoPFCandidateedmPtredmValueMap', 'particleFlow', 'muons', 'RECO', '76.1502', '30.8237'], ['recoCaloClusters', 'hybridSuperClusters', 'uncleanOnlyHybridBarrelBasicClusters', 'RECO', '84.3099', '30.691'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByTaNC', '', 'RECO', '90.7721', '30.039'], ['recoPreshowerClusterShapes', 'multi5x5PreshowerClusterShape', 'multi5x5PreshowerYClustersShape', 'RECO', '114.073', '28.0637'], ['recoBeamHaloSummary', 'BeamHaloSummary', '', 'RECO', '119.968', '27.243'], ['DcsStatuss', 'scalersRawToDigi', '', 'RECO', '70.8524', '26.6951'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByTancRaw', '', 'RECO', '90.6708', '26.6343'], ['recoPFCandidateedmPtredmValueMap', 'particleFlow', 'photons', 'RECO', '61.123', '26.5447'], ['recoCaloClusters', 'hfEMClusters', '', 'RECO', '76.5204', '26.1356'], ['recoPreshowerClusters', 'pfPhotonTranslator', 'pfphot', 'RECO', '108.194', '24.6821'], ['recoBeamSpot', 'offlineBeamSpot', '', 'RECO', '338.425', '24.5404'], ['recoPreshowerClusterShapes', 'multi5x5PreshowerClusterShape', 'multi5x5PreshowerXClustersShape', 'RECO', '114.073', '23.6579'], ['recoGsfElectronCores', 'uncleanedOnlyGsfElectronCores', '', 'RECO', '56.2353', '23.2884'], ['l1extraL1HFRingss', 'l1extraParticles', '', 'RECO', '108.908', '23.114'], ['recoTracksToOnerecoTracksAssociation', 'tevMuons', 'firstHit', 'RECO', '52.7266', '21.7409'], ['recoTracksToOnerecoTracksAssociation', 'tevMuons', 'default', 'RECO', '52.6708', '21.6777'], ['recoTracksToOnerecoTracksAssociation', 'tevMuons', 'picky', 'RECO', '52.5592', '21.556'], ['recoTracksToOnerecoTracksAssociation', 'tevMuons', 'dyt', 'RECO', '52.4477', '21.4488'], ['HFRecHitsSorted', 'reducedHcalRecHits', 'hfreco', 'RECO', '45.2622', '21.2107'], ['recoSuperClustersToOnerecoHFEMClusterShapesAssociation', 'hfEMClusters', '', 'RECO', '49.4469', '19.7377'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'simpleSecondaryVertexHighEffBJetTags', '', 'RECO', '69.3298', '18.9723'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByECALIsolationUsingLeadingPion', '', 'RECO', '91.4054', '18.7352'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByTightElectronRejection', '', 'RECO', '90.9748', '18.4144'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByECALIsolation', '', 'RECO', '91.0001', '18.3237'], ['HORecHitsSorted', 'reducedHcalRecHits', 'horeco', 'RECO', '43.7941', '18.3156'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByTightElectronRejection', '', 'RECO', '91.0508', '18.2568'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationAgainstElectron', '', 'RECO', '91.0001', '18.1506'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByLeadingTrackFinding', '', 'RECO', '91.1521', '18.1372'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationAgainstMuon', '', 'RECO', '90.8988', '18.0364'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByTanc', '', 'RECO', '90.5948', '17.9526'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByMediumElectronRejection', '', 'RECO', '91.0761', '17.8158'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByMediumElectronRejection', '', 'RECO', '91.0001', '17.7303'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByTaNCfrQuarterPercent', '', 'RECO', '91.1774', '17.7283'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByTaNCfrOnePercent', '', 'RECO', '91.0761', '17.7276'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByTaNCfrTenthPercent', '', 'RECO', '91.1268', '17.6771'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByTaNCfrHalfPercent', '', 'RECO', '91.1014', '17.6432'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByTrackIsolationUsingLeadingPion', '', 'RECO', '91.4308', '17.5407'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByMVAElectronRejection', '', 'RECO', '90.9241', '17.4731'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByTightMuonRejection', '', 'RECO', '90.8734', '17.2349'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByIsolationUsingLeadingPion', '', 'RECO', '91.3041', '17.1683'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByTrackIsolation', '', 'RECO', '91.0254', '17.139'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'simpleSecondaryVertexHighPurBJetTags', '', 'RECO', '69.3298', '17.0097'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByLeadingTrackPtCut', '', 'RECO', '91.1014', '16.974'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByLeadingPionPtCut', '', 'RECO', '91.0761', '16.8657'], ['recoPFTauDiscriminator', 'shrinkingConePFTauDiscriminationByIsolation', '', 'RECO', '90.8988', '16.7489'], ['edmErrorSummaryEntrys', 'logErrorHarvester', '', 'RECO', '57.7181', '16.7249'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByVLooseIsolation', '', 'RECO', '90.8734', '16.7006'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByLooseElectronRejection', '', 'RECO', '91.0508', '16.5204'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByLooseIsolation', '', 'RECO', '90.8481', '16.4877'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByLooseElectronRejection', '', 'RECO', '90.9748', '16.4567'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByLeadingTrackPtCut', '', 'RECO', '90.9241', '16.1867'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByMediumIsolation', '', 'RECO', '90.8734', '16.0214'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByLeadingTrackFinding', '', 'RECO', '90.9748', '16.008'], ['recoCaloTauDiscriminator', 'caloRecoTauDiscriminationAgainstMuon', '', 'RECO', '69.6441', '15.9493'], ['recoCaloTauDiscriminator', 'caloRecoTauDiscriminationByLeadingTrackFinding', '', 'RECO', '69.8974', '15.939'], ['recoCaloTauDiscriminator', 'caloRecoTauDiscriminationByECALIsolation', '', 'RECO', '69.7454', '15.9317'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByTightIsolation', '', 'RECO', '90.8481', '15.9044'], ['recoCaloTauDiscriminator', 'caloRecoTauDiscriminationAgainstElectron', '', 'RECO', '69.7454', '15.8953'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByVLooseChargedIsolation', '', 'RECO', '90.9748', '15.7747'], ['recoCaloTauDiscriminator', 'caloRecoTauDiscriminationByLeadingTrackPtCut', '', 'RECO', '69.8468', '15.7432'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByDecayModeSelection', '', 'RECO', '90.9494', '15.7377'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByVLooseCombinedIsolationDBSumPtCorr', '', 'RECO', '91.2788', '15.6016'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByVLooseIsolationDBSumPtCorr', '', 'RECO', '91.0761', '15.5968'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByLooseChargedIsolation', '', 'RECO', '90.9494', '15.5614'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'softElectronByIP3dBJetTags', '', 'RECO', '69.092', '15.5073'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr', '', 'RECO', '91.2534', '15.4546'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByMediumChargedIsolation', '', 'RECO', '90.9748', '15.4496'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'softElectronByPtBJetTags', '', 'RECO', '69.0444', '15.3961'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByLooseIsolationDBSumPtCorr', '', 'RECO', '91.0508', '15.3572'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr', '', 'RECO', '91.2788', '15.3368'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByVLooseIsolation', '', 'RECO', '90.7974', '15.3109'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr', '', 'RECO', '91.2534', '15.2272'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'softMuonByIP3dBJetTags', '', 'RECO', '68.9969', '15.2216'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByTightChargedIsolation', '', 'RECO', '90.9494', '15.1874'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByMediumIsolationDBSumPtCorr', '', 'RECO', '91.0761', '15.1548'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByLeadingPionPtCut', '', 'RECO', '90.8988', '15.1297'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'softMuonByPtBJetTags', '', 'RECO', '68.9493', '15.1176'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByLooseIsolation', '', 'RECO', '90.7721', '15.095'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByDecayModeFinding', '', 'RECO', '90.8228', '15.0903'], ['recoJetedmRefToBaseProdTofloatsAssociationVector', 'softMuonBJetTags', '', 'RECO', '68.8542', '15.0188'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByTancMedium', '', 'RECO', '90.7468', '15.0184'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByTancVLoose', '', 'RECO', '90.7468', '15.0178'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByTancLoose', '', 'RECO', '90.7214', '15.0072'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByTancTight', '', 'RECO', '90.7214', '15.0061'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByTightIsolationDBSumPtCorr', '', 'RECO', '91.0508', '14.9207'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByMediumIsolation', '', 'RECO', '90.7974', '14.8497'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByTightMuonRejection', '', 'RECO', '90.9494', '14.8468'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByMediumMuonRejection', '', 'RECO', '90.8988', '14.8353'], ['recoPFTauDiscriminator', 'hpsTancTausDiscriminationByLooseMuonRejection', '', 'RECO', '90.9494', '14.8158'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByLooseMuonRejection', '', 'RECO', '90.8734', '14.8026'], ['recoPFTauDiscriminator', 'hpsPFTauDiscriminationByTightIsolation', '', 'RECO', '90.7721', '14.658'], ['recoCaloTauDiscriminator', 'caloRecoTauDiscriminationByTrackIsolation', '', 'RECO', '69.7708', '14.4248'], ['recoCaloTauDiscriminator', 'caloRecoTauDiscriminationByIsolation', '', 'RECO', '69.6441', '14.1254'], ['doubleedmValueMap', 'muons', 'muPFIsoValueChargedAll04', 'RECO', '57.089', '14.0556'], ['L1AcceptBunchCrossings', 'scalersRawToDigi', '', 'RECO', '35.7729', '14.0107'], ['doubleedmValueMap', 'muons', 'muPFIsoValueCharged04', 'RECO', '57.0437', '13.861'], ['doubleedmValueMap', 'muons', 'muPFIsoValueChargedAll03', 'RECO', '57.089', '13.7049'], ['doubleedmValueMap', 'muons', 'muPFIsoValueCharged03', 'RECO', '57.0437', '13.5093'], ['doubleedmValueMap', 'muons', 'muPFIsoValueGamma04', 'RECO', '57.0134', '12.171'], ['doubleedmValueMap', 'muons', 'muPFIsoValueGamma03', 'RECO', '57.0134', '11.9733'], ['doubleedmValueMap', 'muons', 'muPFIsoValueNeutral04', 'RECO', '57.0437', '10.8172'], ['doubleedmValueMap', 'muons', 'muPFIsoValueNeutral03', 'RECO', '57.0437', '10.1093'], ['double', 'kt6PFJets', 'rho', 'RECO', '10.8237', '9.45422'], ['double', 'kt6PFJets', 'sigma', 'RECO', '10.8539', '9.42589'], ['PileupSummaryInfos', 'addPileupInfo', '', 'HLT', '423.299', '8.70322'], ['double', 'kt6CaloJetsCentral', 'sigma', 'RECO', '10.9899', '7.96133'], ['double', 'kt6CaloJetsCentral', 'rho', 'RECO', '10.9597', '7.93189'], ['double', 'fixedGridRhoFastjetAll', '', 'RECO', '10.9748', '7.78911'], ['floatedmValueMap', 'eidTight', '', 'RECO', '58.0632', '7.76144'], ['floatedmValueMap', 'eidLoose', '', 'RECO', '58.0632', '7.72467'], ['floatedmValueMap', 'eidRobustLoose', '', 'RECO', '58.1539', '7.42833'], ['booledmValueMap', 'PhotonIDProd', 'PhotonCutBasedIDLooseEM', 'RECO', '55.4623', '7.425'], ['booledmValueMap', 'muons', 'muidTMLastStationOptimizedLowPtTight', 'RECO', '48.6751', '7.36611'], ['doubleedmValueMap', 'muons', 'muPFIsoValuePU04', 'RECO', '56.9681', '7.35856'], ['booledmValueMap', 'muons', 'muidTMLastStationOptimizedLowPtLoose', 'RECO', '48.6751', '7.35278'], ['booledmValueMap', 'PhotonIDProd', 'PhotonCutBasedIDLoose', 'RECO', '55.4321', '7.30778'], ['doubleedmValueMap', 'muons', 'muPFIsoValuePU03', 'RECO', '56.9681', '7.303'], ['floatedmValueMap', 'eidRobustHighEnergy', '', 'RECO', '58.2294', '7.27667'], ['booledmValueMap', 'PhotonIDProd', 'PhotonCutBasedIDTight', 'RECO', '55.4321', '7.26067'], ['booledmValueMap', 'muons', 'muidTMLastStationAngTight', 'RECO', '48.5089', '7.21922'], ['booledmValueMap', 'muons', 'muidTM2DCompatibilityTight', 'RECO', '48.524', '7.212'], ['booledmValueMap', 'muons', 'muidTMLastStationAngLoose', 'RECO', '48.5089', '7.21178'], ['booledmValueMap', 'muons', 'muidTM2DCompatibilityLoose', 'RECO', '48.524', '7.21144'], ['floatedmValueMap', 'eidRobustTight', '', 'RECO', '58.1539', '7.18878'], ['booledmValueMap', 'muons', 'muidTMOneStationAngTight', 'RECO', '48.4938', '7.18767'], ['booledmValueMap', 'muons', 'muidTMOneStationAngLoose', 'RECO', '48.4938', '7.17978'], ['booledmValueMap', 'muons', 'muidGMStaChiCompatibility', 'RECO', '48.5089', '7.14733'], ['booledmValueMap', 'muons', 'muidTMLastStationTight', 'RECO', '48.4636', '7.14589'], ['booledmValueMap', 'muons', 'muidTMLastStationLoose', 'RECO', '48.4636', '7.14356'], ['booledmValueMap', 'muons', 'muidTrackerMuonArbitrated', 'RECO', '48.5089', '7.14289'], ['booledmValueMap', 'muons', 'muidGlobalMuonPromptTight', 'RECO', '48.5089', '7.13656'], ['booledmValueMap', 'muons', 'muidTMOneStationTight', 'RECO', '48.4484', '7.12089'], ['booledmValueMap', 'muons', 'muidTMOneStationLoose', 'RECO', '48.4484', '7.09433'], ['booledmValueMap', 'muons', 'muidGMTkChiCompatibility', 'RECO', '48.4938', '7.08078'], ['booledmValueMap', 'muons', 'muidGMTkKinkTight', 'RECO', '48.388', '6.97567'], ['booledmValueMap', 'muons', 'muidAllArbitrated', 'RECO', '48.388', '6.94478'], ['uintedmValueMap', 'muons', 'cosmicsVeto', 'RECO', '51.9712', '6.70878'], ['double', 'fixedGridRhoAll', '', 'RECO', '10.869', '6.037'], ['double', 'kt6CaloJets', 'sigma', 'RECO', '10.8841', '6.01933'], ['double', 'kt6CaloJets', 'rho', 'RECO', '10.8539', '5.98611'], ['edmTriggerResults', 'TriggerResults', '', 'RECO', '82.081', '5.41978'], ['edmTriggerResults', 'TriggerResults', '', 'SIM', '80.0659', '5.28178'], ['edmConditionsInEventBlock', 'conditionsInEdm', '', 'RECO', '10.9669', '4.42589'], ['doubles', 'ak7BasicJets', 'sigmas', 'RECO', '16.9886', '4.30956'], ['doubles', 'ak5TrackJets', 'sigmas', 'RECO', '16.9886', '4.30956'], ['doubles', 'kt6CaloJets', 'sigmas', 'RECO', '16.9734', '4.30867'], ['doubles', 'kt4CaloJets', 'sigmas', 'RECO', '16.9734', '4.30867'], ['doubles', 'ak7CaloJets', 'sigmas', 'RECO', '16.9734', '4.30867'], ['doubles', 'ak5CaloJets', 'sigmas', 'RECO', '16.9734', '4.30867'], ['doubles', 'ak5CaloJets', 'rhos', 'RECO', '16.9432', '4.27878'], ['doubles', 'kt6PFJets', 'sigmas', 'RECO', '16.9432', '4.27878'], ['doubles', 'kt4PFJets', 'sigmas', 'RECO', '16.9432', '4.27878'], ['doubles', 'ak7PFJets', 'sigmas', 'RECO', '16.9432', '4.27878'], ['doubles', 'ak5PFJets', 'sigmas', 'RECO', '16.9432', '4.27878'], ['doubles', 'ak7CaloJets', 'rhos', 'RECO', '16.9432', '4.27878'], ['doubles', 'kt4CaloJets', 'rhos', 'RECO', '16.9432', '4.27878'], ['doubles', 'kt6CaloJets', 'rhos', 'RECO', '16.9432', '4.27878'], ['doubles', 'ak5TrackJets', 'rhos', 'RECO', '16.9583', '4.27622'], ['doubles', 'ak7BasicJets', 'rhos', 'RECO', '16.9583', '4.27622'], ['doubles', 'ak5PFJets', 'rhos', 'RECO', '16.913', '4.23844'], ['doubles', 'kt6PFJets', 'rhos', 'RECO', '16.913', '4.23844'], ['doubles', 'ak7PFJets', 'rhos', 'RECO', '16.913', '4.23844'], ['doubles', 'kt4PFJets', 'rhos', 'RECO', '16.913', '4.23844'], ['double', 'kt6PFJetsCentralNeutral', 'sigma', 'RECO', '11.0654', '3.44633'], ['double', 'kt6PFJetsCentralNeutral', 'rho', 'RECO', '11.0352', '3.41433'], ['double', 'kt6PFJetsCentralChargedPileUp', 'sigma', 'RECO', '11.1561', '2.50444'], ['double', 'kt6PFJetsCentralChargedPileUp', 'rho', 'RECO', '11.1259', '2.47411'], ['double', 'kt6PFJetsCentralNeutralTight', 'sigma', 'RECO', '11.141', '2.47289'], ['double', 'kt6PFJetsCentralNeutralTight', 'rho', 'RECO', '11.1108', '2.44256'], ['double', 'ak5TrackJets', 'sigma', 'RECO', '10.8992', '2.22722'], ['double', 'ak7BasicJets', 'sigma', 'RECO', '10.8992', '2.22722'], ['double', 'ak5CaloJets', 'sigma', 'RECO', '10.8841', '2.21211'], ['double', 'kt4CaloJets', 'sigma', 'RECO', '10.8841', '2.21211'], ['double', 'ak7CaloJets', 'sigma', 'RECO', '10.8841', '2.21211'], ['double', 'ak7BasicJets', 'rho', 'RECO', '10.869', '2.197'], ['double', 'ak5TrackJets', 'rho', 'RECO', '10.869', '2.197'], ['double', 'ak7CaloJets', 'rho', 'RECO', '10.8539', '2.18189'], ['double', 'kt4PFJets', 'sigma', 'RECO', '10.8539', '2.18189'], ['double', 'ak7PFJets', 'sigma', 'RECO', '10.8539', '2.18189'], ['double', 'ak5CaloJets', 'rho', 'RECO', '10.8539', '2.18189'], ['double', 'ak5PFJets', 'sigma', 'RECO', '10.8539', '2.18189'], ['double', 'kt4CaloJets', 'rho', 'RECO', '10.8539', '2.18189'], ['double', 'ak5PFJets', 'rho', 'RECO', '10.8237', '2.15167'], ['double', 'ak7PFJets', 'rho', 'RECO', '10.8237', '2.15167'], ['double', 'kt4PFJets', 'rho', 'RECO', '10.8237', '2.15167']]
	    )
        if self._configDataAccessor:
            self._dataAccessor.addConfig(self._configDataAccessor)
        if self._eventContent:
            self._dataAccessor.addBranches(self._eventContent[0], self._eventContent[1])
        self._eventContentView.updateContent()
    
    def selectInputFile(self):
        filename = str(QFileDialog.getOpenFileName(self,
                                               'Select input file',
                                               QCoreApplication.instance().getLastOpenLocation(),
                                               "Dataformat file / EDM root file (*.txt *.root)"))
        if filename!="":
            self._inputFileName=filename
            self.updateContent()

    def help(self):
        QMessageBox.about(self, 'Info', self._helpMessage)
        
