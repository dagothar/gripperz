#include "XMLHelpers.hpp"

#include <sstream>
#include <boost/filesystem/path.hpp>



using namespace gripperz::util;

using namespace std;
USE_ROBWORK_NAMESPACE;
using namespace robwork;
using namespace rwsim;
using namespace boost::numeric;
using namespace boost::filesystem;
using namespace boost::property_tree;

std::pair<bool, int> toInt(const std::string& str) {
    std::pair<bool, int> nothing(false, 0);
    istringstream buf(str);
    int x;
    buf >> x;
    if (!buf) return nothing;
    string rest;
    buf >> rest;
    if (buf) return nothing;
    else return make_pair(true, x);
}

std::pair<bool, double> toDouble(const std::string& str) {
    std::pair<bool, double> nothing(false, 0);
    istringstream buf(str);
    string number;
    buf >> number;
    double x = boost::lexical_cast<double>(number);
    if (!buf) return nothing;
    string rest;
    buf >> rest;
    if (buf) return nothing;
    else return make_pair(true, x);
}

std::vector<double> XMLHelpers::readArray(PTree& tree) {
    istringstream buf(tree.get_value<string>());
    std::vector<double> values;

    std::string str;
    while (buf >> str) {
        const pair<bool, double> okNum = toDouble(str);
        if (!okNum.first)
            RW_THROW("Number expected. Got \"" << str << "\" ");
        values.push_back(okNum.second);
    }

    return values;
}

double XMLHelpers::readInt(PTree& tree) {
    string str = tree.get_value<string>();
    pair<bool, int> okNum = toInt(str);

    if (!okNum.first)
        RW_THROW("Number expected. Got \"" << str << "\" ");

    return okNum.second;
}

double XMLHelpers::readDouble(PTree& tree) {
    string str = tree.get_value<string>();
    pair<bool, double> okNum = toDouble(str);

    if (!okNum.first)
        RW_THROW("Number expected. Got \"" << str << "\" ");

    return okNum.second;
}

Q XMLHelpers::readQ(PTree& tree) {
    std::vector<double> arr = readArray(tree);
    Q q(arr.size());

    for (size_t i = 0; i < q.size(); i++) {
        q[i] = arr[i];
    }

    return q;
}

std::string XMLHelpers::QToString(rw::math::Q q) {
    stringstream sstr;

    sstr << ' ';

    for (unsigned i = 0; i < q.size(); ++i) {
        sstr << q(i) << ' ';
    }

    return sstr.str();
}

PTree XMLHelpers::parseXMLFile(const std::string& filename) {
    PTree tree;

    try {
        boost::filesystem::path p(filename);
        read_xml(filename, tree);

    } catch (const ptree_error& e) {
        RW_THROW(e.what());
    }

    return tree;
}

void XMLHelpers::saveXMLFile(const std::string& filename, const PTree& tree) {
    try {
        boost::property_tree::xml_writer_settings<char> settings('\t', 1);
        write_xml(filename, tree, std::locale(), settings);
    } catch (const ptree_error& e) {
        RW_THROW(e.what());
    }
}
