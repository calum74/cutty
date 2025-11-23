/*
	Written by Calum Grant
	See https://github.com/calum74/Properties
	Copyright (C) Calum Grant 2019.
	Licensed under Apache 2.0.
*/

#pragma once

#define CY_PROPERTY(Outer, Name, Type) \
	struct Name##_type { \
		Type Get() const { return get_Object().get_##Name(); } \
		void Set(Type value) { get_Object().set_##Name(value); } \
		Name##_type & operator=(Type value) { Set(value); return *this; } \
		operator Type() const { return Get(); } \
		Type operator*() const { return Get(); } \
    private: \
		const Outer & get_Object() const { return *reinterpret_cast<const Outer*>(reinterpret_cast<const char*>(this) - (reinterpret_cast<const char*>(&((Outer*)nullptr)->Name) - reinterpret_cast<char*>((Outer*)nullptr))); } \
		Outer & get_Object() { return *reinterpret_cast<Outer*>(reinterpret_cast<char*>(this) - (reinterpret_cast<char*>(&((Outer*)nullptr)->Name) - reinterpret_cast<char*>((Outer*)nullptr))); } \
        friend Outer; \
        Name##_type() { } \
        Name##_type(const Name##_type&) { } \
		Name##_type & operator=(const Name##_type & src) { return *this; } \
	} Name;

#define CY_GETTER(Outer, Name, Type) \
	struct Name##_type { \
		Type Get() const { return get_Object().get_##Name(); } \
		operator Type() const { return Get(); } \
		Type operator*() const { return Get(); } \
    private: \
		const Outer & get_Object() const { return *reinterpret_cast<const Outer*>(reinterpret_cast<const char*>(this) - (reinterpret_cast<const char*>(&((Outer*)nullptr)->Name) - reinterpret_cast<char*>((Outer*)nullptr))); } \
        friend Outer; \
        Name##_type() { } \
        Name##_type(const Name##_type&) { } \
		Name##_type & operator=(const Name##_type & src) { return *this; } \
	} Name;

#define CY_SETTER(Outer, Name, Type) \
	struct Name##_type { \
		void Set(Type value) { get_Object().set_##Name(value); } \
		Name##_type & operator=(Type value) { Set(value); return *this; } \
    private: \
		Outer & get_Object() { return *reinterpret_cast<Outer*>(reinterpret_cast<char*>(this) - (reinterpret_cast<char*>(&((Outer*)nullptr)->Name) - reinterpret_cast<char*>((Outer*)nullptr))); } \
        friend Outer; \
        Name##_type() { } \
        Name##_type(const Name##_type&) { } \
		Name##_type & operator=(const Name##_type & src) { return *this; } \
	} Name;
