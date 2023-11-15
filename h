boost::hana::tuple<
    boost::hana::tuple<
        boost::hana::type_impl<summer::dag::graph::Vertex<BeanResolverA>>::_, 
        boost::hana::type_impl<summer::dag::graph::Vertex<summer::dag::util::ChangeableBeanResolver<BeanResolverB, boost::hana::tuple<boost::hana::type_impl<A>::_>>>>::_, 
        boost::hana::type_impl<summer::dag::graph::Vertex<summer::dag::util::ChangeableBeanResolver<summer::dag::util::ChangeableBeanResolver<BeanResolverC, boost::hana::tuple<boost::hana::type_impl<A>::_>>, 
    boost::hana::tuple<boost::hana::type_impl<B>::_>>>>::_>, boost::hana::detail::map_impl<boost::hana::detail::hash_table<boost::hana::detail::bucket<A, 0ul>, boost::hana::detail::bucket<B, 1ul>, boost::hana::detail::bucket<C, 2ul>>, 
        boost::hana::basic_tuple<
        boost::hana::pair<boost::hana::type_impl<A>::_, boost::hana::tuple<boost::hana::type_impl<AImpl>::_>>, 
        boost::hana::pair<boost::hana::type_impl<B>::_, boost::hana::tuple<boost::hana::type_impl<BImpl>::_>>, 
        boost::hana::pair<boost::hana::type_impl<C>::_, boost::hana::tuple<boost::hana::type_impl<CImpl>::_>>>>, 
    boost::hana::tuple<>>