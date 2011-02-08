/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "gdcmMaximumLengthSub.h"
#include "gdcmSwapper.h"

namespace gdcm
{
namespace network
{
const uint8_t MaximumLengthSub::ItemType = 0x51;
const uint8_t MaximumLengthSub::Reserved2 = 0x00;

MaximumLengthSub::MaximumLengthSub()
{
  ItemLength = 0x4;
  MaximumLength = 0x4000;
  assert( (size_t)ItemLength + 4 == Size() );
}

std::istream &MaximumLengthSub::Read(std::istream &is)
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

  uint32_t maximumlength;
  is.read( (char*)&maximumlength, sizeof(MaximumLength) );
  SwapperDoOp::SwapArray(&maximumlength,1);
  MaximumLength = maximumlength; // 16384 == max possible (0x4000)

  return is;
}

const std::ostream &MaximumLengthSub::Write(std::ostream &os) const
{
  os.write( (char*)&ItemType, sizeof(ItemType) );
  os.write( (char*)&Reserved2, sizeof(Reserved2) );
  //os.write( (char*)&ItemLength, sizeof(ItemLength) );
  {
  uint16_t copy = ItemLength;
  SwapperDoOp::SwapArray(&copy,1);
  os.write( (char*)&copy, sizeof(ItemLength) );
  }

  //os.write( (char*)&MaximumLength, sizeof(MaximumLength) );
  {
  uint32_t copy = MaximumLength;
  SwapperDoOp::SwapArray(&copy,1);
  os.write( (char*)&copy, sizeof(MaximumLength) );
  }

  return os;
}

size_t MaximumLengthSub::Size() const
{
  size_t ret = 0;
  ret += sizeof(ItemType);
  ret += sizeof(Reserved2);
  ret += sizeof(ItemLength);
  ret += sizeof(MaximumLength);

  return ret;
}

} // end namespace network
} // end namespace gdcm