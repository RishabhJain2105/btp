package ngapType

import "my5G-RANTester/lib/aper"

// Need to import "free5gc/lib/aper" if it uses "aper"

type PDUSessionResourceSetupItemSURes struct {
	PDUSessionID                            PDUSessionID
	PDUSessionResourceSetupResponseTransfer aper.OctetString
	IEExtensions                            *ProtocolExtensionContainerPDUSessionResourceSetupItemSUResExtIEs `aper:"optional"`
}
