//
// Created by rhantolq on 26.10.2019.
//

//#include "poset.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

namespace {

     using Relations = unordered_set<const string*>;
    // Poset class.
    // Every string in this map represents a Poset element and every element has
    // it's incoming relations stored as first element of the pair and outgoing
    // relations as the second element of the pair.
    using Poset = unordered_map<string, pair<Relations, Relations>>;

    unsigned long id_counter = 0;

    unordered_map<unsigned long, Poset> posets;

    void poset_add_out(Relations *additional_out_set, const string *additional_element,
						const string current_element, Poset *poset) {
		
		auto *el = &(poset->find(current_element)->second);
		if (el->second.find(additional_element) == el->second.end()) {
			for (auto iterator = additional_out_set->begin(); iterator != additional_out_set->end(); ++iterator) {
				el->second.emplace(*iterator);
			}
			el->second.emplace(additional_element);

			for (auto iterator = el->first.begin(); iterator != el->first.end(); ++iterator) {
				poset_add_out(additional_out_set, additional_element, **iterator, poset);
			}
		}

	}

	void poset_add_in(Relations *additional_in_set, const string *additional_element,
						const string current_element, Poset *poset) {
		
		auto *el = &(poset->find(current_element)->second);
		if (el->first.find(additional_element) == el->first.end()) {
			for (auto iterator = additional_in_set->begin(); iterator != additional_in_set->end(); ++iterator) {
				el->first.emplace(*iterator);
			}
			el->first.emplace(additional_element);

			for (auto iterator = el->second.begin(); iterator != el->second.end(); ++iterator) {
				poset_add_in(additional_in_set, additional_element, **iterator, poset);
			}
		}
	}

}

namespace jnp1 {

    unsigned long poset_new(void) {
        id_counter++;
        Poset poset;
        posets[id_counter] = poset;
        return id_counter;
    }


    size_t poset_size(unsigned long id) {
        auto found_poset = posets.find(id);
        if (found_poset == posets.end()) {
            return 0;
        }
        else {
            Poset elements = found_poset->second;
            return elements.size();
        }
    }


    void poset_delete(unsigned long id) {
        auto found = posets.find(id);
        if (found != posets.end()) {
            posets.erase(found);
        }
    }


    bool poset_insert(unsigned long id, char const *value) {
        if (value == NULL) {
            return false;
        }

        auto found_poset = posets.find(id);
        if (found_poset == posets.end()) {
            return false;
        }
        else {
            string element(value);
            pair<Relations, Relations> inout_relations;
            auto result = found_poset->second.emplace(element, inout_relations);
            return result.second;
        }
    }


    bool poset_remove(unsigned long id, char const *value) {
        if (value == NULL) {
            return false;
        }

        if (posets.find(id) == posets.end()) {
            return false;
        }
        Poset& poset = posets[id];
        string val = value;
        if (poset.find(val) == posets[id].end()) {
            return false;
        }
        const string *name_ref = &(poset.find(val)->first);
        auto relations = poset[val];
        auto it = relations.first.begin();
        while (it != relations.first.end()) {
            poset[*(*it)].second.erase(name_ref);
            ++it;
        }
        auto it2 = relations.second.begin();
        while (it2 != relations.second.end()) {
            poset[*(*it2)].first.erase(name_ref);
            ++it2;
        }
        poset.erase(val);
        return true;
    }


    bool poset_del(unsigned long id, char const *value1, char const *value2) {
        auto found_poset = posets.find(id);
        if (found_poset == posets.end() || value1 == NULL || value2 == NULL) {
            return false;
        }
        Poset& poset = found_poset->second;
        string val1 = value1;
        string val2 = value2;
        auto elem1 = poset.find(val1);
        auto elem2 = poset.find(val2);
        if (elem1 == poset.end() || elem2 == poset.end()) {
            return false;
        }

        Relations& elem1out = elem1->second.second;
        Relations& elem2in = elem2->second.first;

        if (elem1out.find(&(elem2->first)) == elem1out.end()) {
            return false;
        }

        auto it = elem1out.begin();
        while (it != elem1out.end()) {
            if (elem2in.find(*it) != elem2in.end()) {
                return false;
            }
            ++it;
        }
        elem1out.erase(&(elem2->first));
        elem2in.erase(&(elem1->first));
        return true;
    }

    bool poset_test(unsigned long id, char const *value1, char const *value2) {
        auto found_poset = posets.find(id);
        if (found_poset == posets.end() || value1 == NULL || value2 == NULL) {
            return false;
        }
        Poset& poset = found_poset->second;
        string val1 = value1;
        string val2 = value2;
        auto elem1 = poset.find(val1);
        auto elem2 = poset.find(val2);
        if (elem1 == poset.end() || elem2 == poset.end()) {
            return false;
        }
        return elem1->second.second.find(&(elem2->first)) != elem1->second.second.end() || elem1 == elem2;
    }

    void poset_clear(unsigned long id) {
        posets[id].clear();
    }


	bool poset_add(unsigned long id, char const *value1, char const *value2) {
		unordered_map<unsigned long, Poset>::iterator found_poset;
		found_poset = posets.find(id);
		Poset::iterator found_rel1;
		Poset::iterator found_rel2;

		if (found_poset == posets.end() || value1 == NULL || value2 == NULL) {
			
			return false;
		}

		found_rel1 = (found_poset->second).find(value1);
		found_rel2 = (found_poset->second).find(value2);
		if (found_rel1 == (found_poset->second).end() || found_rel2 == (found_poset->second).end()){
			return false;
		}
		else if (found_rel1->second.second.find(&(found_rel2->first)) != found_rel1->second.second.end() ||
                    found_rel2->second.second.find(&(found_rel1->first)) != found_rel2->second.second.end()) {
			return false;
		}
		else {
			poset_add_out(&(found_rel2->second.second), &(found_rel2->first), found_rel1->first, &(found_poset->second));
			poset_add_in(&(found_rel1->second.first), &(found_rel1->first), found_rel2->first, &(found_poset->second));
			return true;
		}
	}


}
#include <cassert>
using namespace std;
using namespace jnp1;
int main() {

    unordered_set<string> relations;
    unordered_map<string, unordered_set<string>> elements;
    unordered_map<string, int> mapa;
//	mapa["asd"]= 1;
//	string str2 = &(mapa.find("asd")->first);
//	cout<<str2<<endl;

/*
    unsigned long id =  jnp1::poset_new();
    cout << "id = " << id << endl;
    cout << (jnp1::poset_size(id))<<endl;
    cout << jnp1::poset_insert(id, "abc") << endl;
    cout<<jnp1::poset_insert(0, "aabc") << endl;
    cout << "Size of poset id1 = " << (jnp1::poset_size(id)) << endl;
    unsigned long id_test = jnp1::poset_new();
    cout << jnp1::poset_insert(id_test, "siema") << endl;
    cout << jnp1::poset_insert(id_test, "siema") << endl;
    cout << jnp1::poset_insert(id_test, "siema2") << endl;
    auto it1 = jnp1::posets[id_test].find("siema");
    auto it2 = jnp1::posets[id_test].find("siema2");
    it1->second.second.insert(&(it2->first));
    it2->second.first.insert(&(it1->first));
    cout<< "Assert siema r siema2 = " <<jnp1::poset_test(id_test, "siema", "siema2") << endl;
    cout << "Assert del siema r siema2 = " << jnp1::poset_del(id_test, "siema", "siema2") << endl;
    cout << "Assert ! siema r siema2 = " << jnp1::poset_test(id_test, "siema", "siema2") << endl;
    cout<< "Size id_test = " << jnp1::poset_size(id_test) << endl;
    cout<< "Assert !in id1 siema r siema2 " <<jnp1::poset_test(id, "siema", "siema2") << endl;
    jnp1::poset_delete(id);
    jnp1::poset_delete(id_test);*/
  unsigned long p1;

  p1 = poset_new();
  assert(poset_size(p1) == 0);
  assert(poset_size(p1 + 1) == 0);
  assert(!poset_insert(p1, NULL));
  assert(poset_insert(p1, "A"));
  assert(poset_test(p1, "A", "A"));
  assert(!poset_insert(p1, "A"));
  assert(!poset_insert(p1 + 1, "B"));
  assert(poset_size(p1) == 1);
  assert(!poset_remove(p1 + 1, "A"));
  assert(poset_remove(p1, "A"));
  assert(!poset_remove(p1, "A"));
  assert(poset_insert(p1, "B"));
  assert(poset_insert(p1, "C"));
  assert(poset_add(p1, "B", "C"));
  assert(!poset_remove(p1, "A"));
  assert(!poset_add(p1, NULL, "X"));
  assert(!poset_del(p1, NULL, "X"));
  assert(!poset_test(p1, NULL, "X"));
  assert(!poset_add(p1, "X", NULL));
  assert(!poset_del(p1, "X", NULL));
  assert(!poset_test(p1, "X", NULL));
  assert(!poset_add(p1, NULL, NULL));
  assert(!poset_del(p1, NULL, NULL));
  assert(!poset_test(p1, NULL, NULL));
  assert(!poset_add(p1, "C", "D"));
  assert(!poset_add(p1, "D", "C"));
  assert(!poset_del(p1, "C", "D"));
  assert(!poset_del(p1, "D", "C"));
  assert(!poset_test(p1, "C", "D"));
  assert(!poset_test(p1, "D", "C"));
  assert(!poset_add(p1 + 1, "C", "D"));
  assert(!poset_del(p1 + 1, "C", "D"));
  assert(!poset_test(p1 + 1, "C", "D"));
  poset_clear(p1);
  poset_clear(p1 + 1);
  assert(poset_insert(p1, "E"));
  assert(poset_insert(p1, "F"));
  assert(poset_insert(p1, "G"));
  assert(poset_add(p1, "E", "F"));
  assert(!poset_add(p1, "E", "F"));
  assert(!poset_add(p1, "F", "E"));
  assert(poset_test(p1, "E", "F"));
  assert(!poset_test(p1, "F", "E"));
  assert(poset_add(p1, "F", "G"));
  assert(poset_test(p1, "E", "G"));
  assert(!poset_del(p1, "E", "G"));
  assert(poset_del(p1, "E", "F"));
  assert(!poset_del(p1, "E", "F"));
  assert(!poset_del(p1, "G", "F"));
  assert(!poset_del(p1, "G", "G"));
  assert(poset_size(p1) == 3);
  poset_delete(p1);
  poset_delete(p1);
  poset_delete(p1 + 1);
    
    return 0;
}
