  vector<uint64_t> entries_by_tag( const name searchtag )
  {
    vector<uint64_t> ret;
    auto tagidx = _tagcloud.get_index<N(tag)>();
    auto itr = tagidx.lower_bound(searchtag);
    while(itr != tagidx.end() && itr->tag == searchtag ) {
      ret.emplace_back(itr->entry_id);
      itr++;
    }
    return ret;
  }

  vector<name> tags_by_entry( const uint64_t entry_id )
  {
    vector<name> ret;
    auto tagidx = _tagcloud.get_index<N(entryid)>();
    auto itr = tagidx.lower_bound(entry_id);
    while(itr != tagidx.end() && itr->tag == entry_id ) {
      ret.emplace_back(itr->tag);
      itr++;
    }
    return ret;
  }



