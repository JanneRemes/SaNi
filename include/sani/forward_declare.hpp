#pragma once

#define SANI_FORWARD_DECLARE(c) class c;
#define SANI_FORWARD_DECLARE_1(n1, c) namespace n1 { class c; }
#define SANI_FORWARD_DECLARE_2(n1, n2, c) namespace n1 { namespace n2 { class c; } }
#define SANI_FORWARD_DECLARE_3(n1, n2, n3, c) namespace n1 { namespace n2 { namespace n3 { class c; } } }
#define SANI_FORWARD_DECLARE_4(n1, n2, n3, n4, c) namespace n1 { namespace n2 { namespace n3 { namespace n4 { class c; } } } }

#define SANI_FORWARD_DECLARE_STRUCT(c) struct c;
#define SANI_FORWARD_DECLARE_STRUCT_1(n1, c) namespace n1 { struct c; }
#define SANI_FORWARD_DECLARE_STRUCT_2(n1, n2, c) namespace n1 { namespace n2 { struct c; } }
#define SANI_FORWARD_DECLARE_STRUCT_3(n1, n2, n3, c) namespace n1 { namespace n2 { namespace n3 { struct c; } } }
#define SANI_FORWARD_DECLARE_STRUCT_4(n1, n2, n3, n4, c) namespace n1 { namespace n2 { namespace n3 { namespace n4 { struct c; } } } }

#define SANI_FORWARD_DECLARE_ENUM_CLASS(c) struct c;
#define SANI_FORWARD_DECLARE_ENUM_CLASS_1(n1, c) namespace n1 { enum class c; }
#define SANI_FORWARD_DECLARE_ENUM_CLASS_2(n1, n2, c) namespace n1 { namespace n2 { enum class c; } }
#define SANI_FORWARD_DECLARE_ENUM_CLASS_3(n1, n2, n3, c) namespace n1 { namespace n2 { namespace n3 { enum class c; } } }
#define SANI_FORWARD_DECLARE_ENUM_CLASS_4(n1, n2, n3, n4, c) namespace n1 { namespace n2 { namespace n3 { namespace n4 { enum class c; } } } }