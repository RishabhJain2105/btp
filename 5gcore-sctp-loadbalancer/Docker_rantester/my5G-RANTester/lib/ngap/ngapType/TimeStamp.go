package ngapType

import "my5G-RANTester/lib/aper"

// Need to import "free5gc/lib/aper" if it uses "aper"

type TimeStamp struct {
	Value aper.OctetString `aper:"sizeLB:4,sizeUB:4"`
}
