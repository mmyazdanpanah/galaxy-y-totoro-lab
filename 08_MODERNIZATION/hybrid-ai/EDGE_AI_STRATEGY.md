# Totoro Edge AI Strategy

Status: research and implementation strategy; no inference runtime is accepted on physical Totoro yet.

## Core position

Success means running a small, useful, measurable task locally while delegating workloads beyond the Galaxy Y's demonstrated capability. Totoro is an ARMv6-era Android/Linux device with severe CPU, memory and storage constraints. Modern frameworks and model packages cannot be assumed compatible.

## Workload placement

| Workload | Preferred node |
|---|---|
| Deterministic rules, state machine, command allowlist | Totoro |
| Small tabular/sensor classifier | Totoro after runtime proof |
| Tiny keyword/signal classifier | Totoro or iPhone after benchmark |
| Image/audio perception | iPhone or Mac |
| Embeddings, retrieval, small language models | Mac; optionally capable SBC |
| Larger LLM reasoning and agent orchestration | Mac |

This is a placement strategy, not a measured performance ranking.

## Totoro-local candidates

Start with finite-state machines, thresholding, moving averages and simple statistical anomaly detection. Then consider logistic regression, a small decision tree or a tiny fully connected neural network. Small convolutional or keyword-spotting models are later experiments if input capture and compute budgets justify them.

Parameter count alone does not establish fit: activations, runtime overhead, allocator behavior and input buffers also consume memory. Measure the complete path.

## Runtime alternatives

### Custom C inference engine
A small C runtime compiled for the verified ARMv6 environment offers control over supported operators, allocation and integer arithmetic. Begin with a narrow operator set and simple versioned model representation. Compare outputs against a Mac reference.

### TensorFlow Lite Micro
Use TFLM as an architectural reference or possible porting base, not as an assumed drop-in runtime. Investigate C++ requirements, compiler assumptions, kernels, memory planning and build dependencies first.

### Other compact libraries
Evaluate only against concrete criteria: ARMv6-compatible code generation, no mandatory NEON, operator coverage, linking, memory control, license and reproducibility. A generic ARM label is insufficient evidence.

### iPhone Core ML
Use the iPhone for tasks impractical on Totoro but supported by the selected iOS/Core ML target. Verify model compatibility, memory, latency, energy and permissions on the physical phone.

### Mac inference
Use the Mac for language models, embeddings and complex tasks. Keep the Totoro-facing API independent of the model runtime.

### Separate Linux edge board
Consider an SBC only if standalone local language-model behavior becomes a requirement. Compare total cost, power, integration, supported runtime and measured latency against Mac/iPhone delegation.

## Quantization and budgets

Task-specific models are preferred over general-purpose language models. int8 is a reasonable first classifier experiment, but quantization does not remove activation memory or compute costs. Establish budgets experimentally from free RAM, peak resident memory, stack/heap, input dimensions and worst-case latency.

## Benchmark record

For every candidate record model/source/license/data provenance, task and schemas, input shape, parameter count, file size, quantization, operators, compiler/flags/ABI, runtime revision, reference outputs/tolerance, repeated latency, peak memory or proxy, CPU and power observations, robustness results, and binary/model SHA-256.

Test on Mac first, then a compatible host environment if available, then physical Totoro. A successful host build is not physical acceptance.

## Delegation policy

Run locally only when capability/model are installed and verified, input is within strict limits, data policy permits local execution and latency is acceptable. Otherwise delegate only to an authenticated available peer. Offline, return a truthful deferred/unavailable status or use a defined local fallback.

## Initial experiment

Train a tiny sensor/event classifier on the Mac, implement a minimal C inference path for a narrow operator set, and compare host reference outputs with Totoro results using synthetic inputs. This tests toolchain, arithmetic, memory and task representation without camera, microphone or language-model complexity.

## Exit criteria

Accept edge inference only when one named model performs one defined task on physical Totoro with repeatable correctness, acceptable latency and memory use, reproducible artifacts and safe failure behavior. If it is too slow or does not fit, retain the measurement and place that workload on iPhone or Mac.
