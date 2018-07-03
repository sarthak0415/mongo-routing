require_relative '3dijkstras_mongo'
require "benchmark"

dataset_name = ARGV[0]

#disable mongo logging
Mongo::Logger.logger.level = Logger::FATAL

test_results = {}

test_results[1] = run_tests(dataset_name, 1)
test_results[10] = run_tests(dataset_name, 10)
test_results[50] = run_tests(dataset_name, 50)
test_results[100] = run_tests(dataset_name, 100)

output_file_name = dataset_name + "_test_results.txt"
File.open(output_file_name, 'w') { |file| file.write(test_results) }
