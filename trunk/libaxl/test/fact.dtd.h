/**
 * C inline representation for DTD fact.dtd, created by axl-knife
 */
#ifndef __FACT_DTD_H__
#define __FACT_DTD_H__
#define FACT_DTD "\n\
<!--                                                                                    \
   Copyright (C) 2003  Advanced Software Production Line, S.L.                          \
   Af-Arch Message definition to perform remote service invocation.                     \
  -->                                                                                   \
<!ELEMENT  general_message (protocol_version, (request_service | response_service)) >   \
                                                                                        \
<!ELEMENT  protocol_version (#PCDATA) >                                                 \
                                                                                        \
<!ELEMENT  request_service ((param | paramtable)*) >                                    \
<!ATTLIST  request_service                                                              \
           name CDATA #IMPLIED>                                                         \
                                                                                        \
<!ELEMENT  param EMPTY>                                                                 \
<!ATTLIST  param                                                                        \
           attrib CDATA #IMPLIED                                                        \
           value  CDATA #IMPLIED >                                                      \
                                                                                        \
<!ELEMENT  paramtable (table)>                                                          \
<!ATTLIST  paramtable                                                                   \
           attrib CDATA #IMPLIED >                                                      \
                                                                                        \
<!ELEMENT  response_service (status,table*) >                                           \
                                                                                        \
<!ELEMENT  status EMPTY>                                                                \
<!ATTLIST  status                                                                       \
           code CDATA #IMPLIED                                                          \
           text  CDATA #IMPLIED >                                                       \
                                                                                        \
<!ELEMENT  table (row*) >                                                               \
                                                                                        \
<!ELEMENT  row (data+) >                                                                \
                                                                                        \
<!ELEMENT  data (#PCDATA) >                                                             \
                                                                                        \
                                                                                        \
                                                                                        \
                                                                                        \
                                                                                        \
\n"
#endif
