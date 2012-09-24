
#include "SCHNAPS/SCHNAPS.hpp"
#include "SCHNAPS/Plugins/Contacts/Contacts.hpp"

SCHNAPS_Plugin_BeginDefinitionM("Contacts", "1.0.0");

SCHNAPS_Plugin_AddAllocM("Contacts_Base", SCHNAPS::Plugins::Contacts::Base::Alloc);
SCHNAPS_Plugin_AddAllocM("Contacts_Transmission", SCHNAPS::Plugins::Contacts::Transmission::Alloc);


SCHNAPS_Plugin_EndDefinitionM();
