/*
    Notes:
        Systems need to have two updates. One that updates a chunk
        and one that updates a set of entities.

        The chunk gets incremented by a chunkSize each frame. If the
        chunk size is -1, no chunk updates should be done.

        A system should also have an enable and disable mechanic.
*/