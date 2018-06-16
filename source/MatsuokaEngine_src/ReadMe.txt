========================================================================
    STATIC LIBRARY : MatsuokaEngine
========================================================================

Core engine for musical creativity with central pattern generators
Top level class is MatsuokaEngine

Supports threadsafe operation - parameter changes are queued for execution using the doQueuedActions() method
GUI thread can call parameter update methods (marked QUEUED ACTION) as and when, and these requests will be queued for actioning by the signal thread. I would suggest calling doQueuedActions at the start of each signal vector calculation. 

This library was designed for use in multiple environments - so there's a lot not used in (e.g.) the MAX implementation.
