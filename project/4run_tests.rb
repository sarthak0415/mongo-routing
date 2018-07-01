require_relative '3dijkstras_mongo'
require "benchmark"

#disable mongo logging
Mongo::Logger.logger.level = Logger::FATAL

test_results = {}

test_results[1] = run_tests(1)
test_results[10] = run_tests(10)
test_results[50] = run_tests(50)
test_results[100] = run_tests(100)


File.open('test_results', 'w') { |file| file.write(test_results) }
