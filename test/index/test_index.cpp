#include <catch.hpp>

#include <stdio.h>
#include <climits>
#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>

#include <graphtyper/graph/graph_serialization.hpp>
#include <graphtyper/graph/constructor.hpp>
#include <graphtyper/index/indexer.hpp>
#include <graphtyper/index/rocksdb.hpp>
#include <graphtyper/utilities/type_conversions.hpp>


TEST_CASE("Test index chr1")
{
  using namespace gyper;
  std::stringstream my_graph;
  my_graph << gyper_SOURCE_DIRECTORY << "/test/data/graphs/index_test_chr1.grf";
  std::stringstream my_index;
  my_index << gyper_SOURCE_DIRECTORY << "/test/data/graphs/index_test_chr1";

  gyper::load_graph(my_graph.str().c_str());
  REQUIRE(graph.size() > 0);

  SECTION("All reference kmers are present in the graph")
  {
    std::vector<char> ref = graph.get_all_ref();
    REQUIRE(ref == gyper::to_vec("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCTTTGGA"));
  }

  gyper::index_graph(my_graph.str(), my_index.str());
  gyper::load_index(my_index.str());
  REQUIRE(gyper::index.check());

  SECTION("Test one var kmer are present in the graph")
  {
    std::vector<char> ref = graph.get_first_var();
    REQUIRE(ref == gyper::to_vec("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTGCCCAGGTTTCCCCAGGTTTCCCCTTTGGA"));
  }


  SECTION("All kmers should be have the correct count")
  {
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAG", 0)).size() == 3);
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCTT", 0)).size() == 1);
    REQUIRE(gyper::index.get(gyper::to_uint64("TTCCCCAGGTTTCCCCAGGTTTCCCCTTTGGA", 0)).size() == 1);
    REQUIRE(gyper::index.get(gyper::to_uint64("GGTTTCCCCAGGTTTCCCCAGGTTTGCCCAGG", 0)).size() == 1);
  }


  SECTION("All kmers should have the correct starting index")
  {
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAG", 0))[0].start_index == 1);
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAG", 0))[1].start_index == 11);
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAG", 0))[2].start_index == 21);

    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCTT", 0))[0].start_index == 31);

    REQUIRE(gyper::index.get(gyper::to_uint64("GGTTTCCCCAGGTTTCCCCAGGTTTGCCCAGG", 0))[0].start_index == 12);
  }


  SECTION("All kmers should have the correct end index")
  {
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAG", 0))[0].end_index == 32);
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAG", 0))[1].end_index == 42);
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAG", 0))[2].end_index == 52);

    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCTT", 0))[0].end_index == 62);

    REQUIRE(gyper::index.get(gyper::to_uint64("GGTTTCCCCAGGTTTCCCCAGGTTTGCCCAGG", 0))[0].end_index == 43);
  }

  SECTION("Correct variant id")
  {
    // AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCTTTGGA
    // chr1 37  rs1 C G 0 . .
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAG", 0))[0].variant_id == gyper::INVALID_ID);
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAG", 0))[1].variant_id == 0);
    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAG", 0))[2].variant_id == 0);

    REQUIRE(gyper::index.get(gyper::to_uint64("AGGTTTCCCCAGGTTTCCCCAGGTTTCCCCTT", 0))[0].variant_id == 0);

    //                                                       alt |
    //                                                           v
    REQUIRE(gyper::index.get(gyper::to_uint64("GGTTTCCCCAGGTTTCCCCAGGTTTGCCCAGG", 0))[0].variant_id == 1);
  }
}


TEST_CASE("Test index chr2")
{
  using namespace gyper;
  using gyper::index;

  std::stringstream my_graph;
  my_graph << gyper_SOURCE_DIRECTORY << "/test/data/graphs/index_test_chr2.grf";
  std::stringstream my_index;
  my_index << gyper_SOURCE_DIRECTORY << "/test/data/graphs/index_test_chr2";

  gyper::load_graph(my_graph.str().c_str());
  REQUIRE(graph.size() > 0);

  // The reference is present in the graph
  {
    std::vector<char> ref = graph.get_all_ref();
    REQUIRE(ref == gyper::to_vec("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTGGACCC"));
  }

  gyper::index_graph(my_graph.str(), my_index.str());
  gyper::load_index(my_index.str());
  REQUIRE(gyper::index.check());

  // All kmers should be have the correct count
  {
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0)).size() == 4);
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTGG", 0)).size() == 1);
    REQUIRE(gyper::index.get(gyper::to_uint64("CACCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0)).size() == 2);
    REQUIRE(gyper::index.get(gyper::to_uint64("CCACAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0)).size() == 2);
    REQUIRE(gyper::index.get(gyper::to_uint64("CAACAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0)).size() == 2);
  }

  // All kmers should have the correct starting index
  {
    REQUIRE(index.get(to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[0].start_index == 0);
    REQUIRE(index.get(to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[1].start_index == 0);
    REQUIRE(index.get(to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[2].start_index == 10);
    REQUIRE(index.get(to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[3].start_index == 20);

    REQUIRE(index.get(to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTGG", 0))[0].start_index == 30);
  }

  // All kmers should have the correct end index
  {
    REQUIRE(index.get(to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[0].end_index == 31);
    REQUIRE(index.get(to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[1].end_index == 31);
    REQUIRE(index.get(to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[2].end_index == 41);
    REQUIRE(index.get(to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[3].end_index == 51);

    REQUIRE(index.get(to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTGG", 0))[0].end_index == 61);
  }

  // Correct variant id and num
  {
    // CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCCCCAGGTTTGGACCC
    // chr2 2 rs2 C A 0 . .
    // chr2  3 rs3 C A 0 . .
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[0].variant_id == 0);
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[0].variant_num == 0);
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[1].variant_id == 2);
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[1].variant_num == 0);
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[2].variant_id == gyper::INVALID_ID);
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[2].variant_num == gyper::INVALID_NUM);
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[3].variant_id == gyper::INVALID_ID);
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTCC", 0))[3].variant_num == gyper::INVALID_NUM);

    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTGG", 0))[0].variant_id == gyper::INVALID_ID);
    REQUIRE(gyper::index.get(gyper::to_uint64("CCCCAGGTTTCCCCAGGTTTCCCCAGGTTTGG", 0))[0].variant_num == gyper::INVALID_NUM);
  }
}


TEST_CASE("Test index chr3")
{
  using namespace gyper;
  // AAAACAAAATAAAACAAAATAAAAGAAAACAAAATAAAACAAAATAAAAGAAAACATTATAAAACA
  // chr3 31 rs4 A G,GA
  std::stringstream my_graph;
  my_graph << gyper_SOURCE_DIRECTORY << "/test/data/graphs/index_test_chr3.grf";
  std::stringstream my_index;
  my_index << gyper_SOURCE_DIRECTORY << "/test/data/graphs/index_test_chr3";

  gyper::load_graph(my_graph.str().c_str());
  REQUIRE(graph.size() > 0);

  gyper::index_graph(my_graph.str(), my_index.str());
  gyper::load_index(my_index.str());
  REQUIRE(gyper::index.check());

  SECTION("All reference kmers are present in the graph")
  {
    std::vector<char> ref = graph.get_all_ref();
    REQUIRE(ref == gyper::to_vec("AAAACAAAATAAAACAAAATAAAAGAAAACAAAATAAAACAAAATAAAAGAAAACATTATAAAACA"));
  }

  SECTION("Kmer that starts at zero is has matches positions with the next one (although this is not a real alignmment)")
  {
    // kmer ref
    std::vector<gyper::KmerLabel> labels0 = gyper::index.get(gyper::to_uint64("AAAACAAAATAAAACAAAATAAAAGAAAACAA", 0));
    REQUIRE(labels0.size() == 1);
    REQUIRE(labels0[0].start_index == 0);
    REQUIRE(labels0[0].end_index == 31);

    // kmer 1
    std::vector<gyper::KmerLabel> labels1 = gyper::index.get(gyper::to_uint64("CAAAATAAAACAAAATAAAAGAAAACGAAAAT", 0));
    REQUIRE(labels1.size() == 1);
    REQUIRE(labels1[0].start_index == 4);

    REQUIRE(labels1[0].end_index == gyper::SPECIAL_START);

    REQUIRE(labels1[0].variant_id == 2);

    // kmer 2
    std::vector<gyper::KmerLabel> labels2 = gyper::index.get(gyper::to_uint64("AAATAAAACAAAATAAAAGAAAACATTATAAA", 0));
    REQUIRE(labels2.size() == 3);
    REQUIRE(labels2[0].start_index == 31);
    REQUIRE(labels2[0].end_index == 62);
    REQUIRE(labels2[0].variant_id == 0);
    REQUIRE(labels2[1].start_index == 31);
    REQUIRE(labels2[1].end_index == 62);
    REQUIRE(labels2[1].variant_id == 1);
    REQUIRE(labels2[2].start_index == 32);
    REQUIRE(labels2[2].end_index == 62);
    REQUIRE(labels2[2].variant_id == 2);

    // kmer 3
    std::vector<gyper::KmerLabel> labels3 = gyper::index.get(gyper::to_uint64("AAAATAAAACAAAATAAAAGAAAACATTATAA", 0));
    REQUIRE(labels3.size() == 2);
    REQUIRE(labels3[0].start_index == 30);
    REQUIRE(labels3[0].end_index == 61);
    REQUIRE(labels3[0].variant_id == 0);
    REQUIRE(labels3[1].start_index == 31);
    REQUIRE(labels3[1].end_index == 61);
    REQUIRE(labels3[1].variant_id == 2);
  }
}
