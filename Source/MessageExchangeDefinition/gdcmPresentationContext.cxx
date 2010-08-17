/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2010 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "gdcmPresentationContext.h"
#include "gdcmUIDs.h"
#include "gdcmSwapper.h"

namespace gdcm
{
namespace network
{
const uint8_t PresentationContext::ItemType = 0x20;
const uint8_t PresentationContext::Reserved2 = 0x00;
const uint8_t PresentationContext::Reserved6 = 0x00;
const uint8_t PresentationContext::Reserved7 = 0x00;
//const uint8_t PresentationContext::Reserved7 = 0xff; // DCMTK ??
const uint8_t PresentationContext::Reserved8 = 0x00;

PresentationContext::PresentationContext()
{
  ID = 0x01;

  ItemLength = Size() - 4;
  assert( ItemLength + 4 == Size() );
}

std::istream &PresentationContext::Read(std::istream &is)
{
  //uint8_t itemtype = 0x0;
  //is.read( (char*)&itemtype, sizeof(ItemType) );
  //assert( itemtype == ItemType );
  uint8_t reserved2;
  is.read( (char*)&reserved2, sizeof(Reserved2) );
  uint16_t itemlength;
  is.read( (char*)&itemlength, sizeof(ItemLength) );
  SwapperDoOp::SwapArray(&itemlength,1);
  ItemLength = itemlength;
  uint8_t id;
  is.read( (char*)&id, sizeof(ID) );
  ID = id;
  uint8_t reserved6;
  is.read( (char*)&reserved6, sizeof(Reserved6) );
  uint8_t reserved7;
  is.read( (char*)&reserved7, sizeof(Reserved7) );
  uint8_t reserved8;
  is.read( (char*)&reserved8, sizeof(Reserved6) );
  SubItems.Read( is );

  TransferSyntax_ ts;
  ts.Read( is );
  TransferSyntaxes.push_back( ts );

  assert( ItemLength + 4 == Size() );
  return is;
}

const std::ostream &PresentationContext::Write(std::ostream &os) const
{
  assert( ItemLength + 4 == Size() );
  os.write( (char*)&ItemType, sizeof(ItemType) );
  os.write( (char*)&Reserved2, sizeof(Reserved2) );
  //os.write( (char*)&ItemLength, sizeof(ItemLength) );
  uint16_t copy = ItemLength;
  SwapperDoOp::SwapArray(&copy,1);
  os.write( (char*)&copy, sizeof(ItemLength) );

  os.write( (char*)&ID, sizeof(ID) );
  os.write( (char*)&Reserved6, sizeof(Reserved6) );
  os.write( (char*)&Reserved7, sizeof(Reserved7) );
  os.write( (char*)&Reserved8, sizeof(Reserved8) );
  SubItems.Write(os);
  std::vector<TransferSyntax_>::const_iterator it = TransferSyntaxes.begin();
  for( ; it != TransferSyntaxes.end(); ++it )
    {
    it->Write( os );
    }

  return os;
}

size_t PresentationContext::Size() const
{
  size_t ret = 0;
  ret += sizeof(ItemType);
  ret += sizeof(Reserved2);
  ret += sizeof(ItemLength);
  ret += sizeof(ID);
  ret += sizeof(Reserved6);
  ret += sizeof(Reserved7);
  ret += sizeof(Reserved8);
  ret += SubItems.Size();
  std::vector<TransferSyntax_>::const_iterator it = TransferSyntaxes.begin();
  for( ; it != TransferSyntaxes.end(); ++it )
    {
    ret += it->Size();
    }

  return ret;
}

void PresentationContext::SetAbstractSyntax( AbstractSyntax const & as )
{
  SubItems = as;
  ItemLength = Size() - 4;
  assert( ItemLength + 4 == Size() );
}

void PresentationContext::AddTransferSyntax( TransferSyntax_ const &ts )
{
  TransferSyntaxes.push_back( ts );
  ItemLength = Size() - 4;
  assert( ItemLength + 4 == Size() );
}

void PresentationContext::SetPresentationContextID( uint8_t id )
{
  ID = id;
}

} // end namespace network
} // end namespace gdcm
