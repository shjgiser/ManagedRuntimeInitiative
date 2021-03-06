/*
 * Copyright 1997-2005 Sun Microsystems, Inc.  All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa Clara,
 * CA 95054 USA or visit www.sun.com if you need additional information or
 * have any questions.
 *  
 */
// This file is a derivative work resulting from (and including) modifications
// made by Azul Systems, Inc.  The date of such changes is 2010.
// Copyright 2010 Azul Systems, Inc.  All Rights Reserved.
//
// Please contact Azul Systems, Inc., 1600 Plymouth Street, Mountain View, 
// CA 94043 USA, or visit www.azulsystems.com if you need additional information 
// or have any questions.
#ifndef FIELDDESCRIPTOR_HPP
#define FIELDDESCRIPTOR_HPP

#include "constantTag.hpp"
#include "constantPoolOop.hpp"
#include "fieldType.hpp"
class fieldDescriptor;

class FieldClosure : public StackObj {
 public:
  FieldClosure() {}
  virtual void do_field_for(fieldDescriptor* fd, oop obj) = 0;
};

// A fieldDescriptor describes the attributes of a single field (instance or class variable).
// It needs the class constant pool to work (because it only holds indices into the pool
// rather than the actual info).

class fieldDescriptor VALUE_OBJ_CLASS_SPEC {
 private:
  AccessFlags         _access_flags;
  int                 _name_index;
  int                 _signature_index;
  int                 _initial_value_index;
  int                 _offset;
  int                 _generic_signature_index;
  int                 _index; // index into fields() array
  constantPoolHandle  _cp;

 public:
  symbolOop name() const               { return _cp->symbol_at(_name_index); }
  symbolOop signature() const          { return _cp->symbol_at(_signature_index); }
  klassOop field_holder() const        { return _cp->pool_holder(); }
  constantPoolOop constants() const    { return _cp(); }
  AccessFlags access_flags() const     { return _access_flags; }
  oop loader() const;
  // Offset (in words) of field from start of instanceOop / klassOop
  int offset() const                   { return _offset; }
  symbolOop generic_signature() const  { return (_generic_signature_index > 0 ? _cp->symbol_at(_generic_signature_index) : (symbolOop)NULL); }
  int index() const                    { return _index; }
  typeArrayOop annotations() const;

  // Initial field value
  bool has_initial_value() const          { return _initial_value_index != 0; }
  constantTag initial_value_tag() const;  // The tag will return true on one of is_int(), is_long(), is_single(), is_double()
  jint        int_initial_value() const;
  jlong       long_initial_value() const;
  jfloat      float_initial_value() const;
  jdouble     double_initial_value() const;
  oop         string_initial_value(TRAPS) const;

  // Field signature type
  BasicType field_type() const            { return FieldType::basic_type(signature()); }

  // Access flags
  bool is_public() const                  { return _access_flags.is_public(); }
  bool is_private() const                 { return _access_flags.is_private(); }
  bool is_protected() const               { return _access_flags.is_protected(); }
  bool is_package_private() const         { return !is_public() && !is_private() && !is_protected(); }

  bool is_static() const                  { return _access_flags.is_static(); }
  bool is_final() const                   { return _access_flags.is_final(); }
  bool is_volatile() const                { return _access_flags.is_volatile(); }
  bool is_transient() const               { return _access_flags.is_transient(); }

  bool is_synthetic() const               { return _access_flags.is_synthetic(); }

  bool is_field_access_watched() const    { return _access_flags.is_field_access_watched(); }
  bool is_field_modification_watched() const
                                          { return _access_flags.is_field_modification_watched(); }
  void set_is_field_access_watched(const bool value)
                                          { _access_flags.set_is_field_access_watched(value); }
  void set_is_field_modification_watched(const bool value)
                                          { _access_flags.set_is_field_modification_watched(value); }

  // Initialization
  void initialize(klassOop k, int index);

  // Print
  void print_on(outputStream* st) const         PRODUCT_RETURN;
  void print_on_for(outputStream* st, oop obj)  PRODUCT_RETURN;

  void print_xml_on_for(xmlBuffer *xb, oop obj);
};

#endif // FIELDDESCRIPTOR_HPP
